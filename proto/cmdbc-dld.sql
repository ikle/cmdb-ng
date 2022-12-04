-- node kinds: group (0), node (1), tag (2), attr (3), value (4)
-- node flags:
--	active (0x80), dirty (0x40) where
--		!active &  dirty = will be added on commit, deleted on discard,
--		 active & !dirty = active node,
--		 active &  dirty = will be deleted on commit
--	changed (0x20) = dirty and all parent chain, reset on commit or discard
--	secure  (0x10) = do not show value in non-configuration mode

CREATE TABLE IF NOT EXISTS tree (
	id	INTEGER PRIMARY KEY,
	parent	INTEGER NOT NULL,
	label	TEXT    NOT NULL,
	schema	INTEGER NOT NULL DEFAULT 0,	-- schema node for tag/value
	kind	INTEGER NOT NULL,
	secure	BOOLEAN NOT NULL DEFAULT 0,	-- secure key or password

	active	BOOLEAN NOT NULL DEFAULT 0,	-- committed
	dirty	BOOLEAN NOT NULL DEFAULT 1,	-- will be added or deleted
	changed	BOOLEAN NOT NULL DEFAULT 1,	-- validate requred

	UNIQUE (parent, label)
);

CREATE INDEX IF NOT EXISTS 'tree-parent'  ON tree (parent);
CREATE INDEX IF NOT EXISTS 'tree-link'    ON tree (link);
CREATE INDEX IF NOT EXISTS 'tree-changed' ON tree (changed);

-- N1. struct cmdbc_node *cmdbc_node_lookup (o, parent, label, active)
	SELECT * FROM tree WHERE parent = $parent AND label = $label
--	 active ? AND active = 1
--	!active ? AND NOT (active = 1 AND dirty = 1)

-- N2. int  cmdbc_node_first (struct cmdbc_node *o, struct cmdbc *s, parent)
--     int  cmdbc_node_next  (struct cmdbc_node *o)
--     void cmdbc_node_done  (struct cmdbc_node *o)
	SELECT * FROM tree WHERE parent = $parent ORDER BY label ASC;

-- M1. long cmdbc_get_parent (o, id)
	SELECT parent FROM tree WHERE id = $id;

-- M2. void cmdbc_mark (o, id)
--	while id ≠ 0:
		UPDATE tree SET changed = 1 WHERE id = $id;
--		id = cmdbc_get_parent (o, id)

-- A1. static long cmdbc_search (o, parent, label)
	SELECT id FROM tree WHERE parent = $parent AND label = $label

-- A2. long cmdbc_add (o, parent, label, kind, schema, secure)
--	cmdbc_search (o, parent, label) or
	INSERT	INTO tree (parent, label, schema, kind, secure)
		VALUES ($parent, $label, $schema, $kind, $secure);
--	cmdbc_mark (o, parent)
--	return id or 0 on error

-- D1. static int cmdbc_node_del_nr (o, id)
	UPDATE tree SET dirty = 1, changed = 1	WHERE id = $id AND active = 1;
	DELETE FROM tree			WHERE id = $id AND active = 0;

-- D2. static int cmdbc_node_del (o, id)
--	for i in yonk_childs (o, id, 0): cmdbc_node_del (o, i)
--	cmdbc_node_del_nr (o, id)

-- D3. int cmdbc_delete (o, id)
--	parent = cmdbc_get_parent (o, id)
--	cmdbc_node_del (o, id)
--	cmdbc_mark (o, parent)

-- C1. int  cmdbc_check_first (struct cmdbc_node *o, struct cmdbc *s, parent)
--     int  cmdbc_check_next  (struct cmdbc_node *o)
--     void cmdbc_check_done  (struct cmdbc_node *o)
	SELECT * FROM tree WHERE parent = $parent AND changed = 1;

-- C2. int cmdbc_commit (o)
	DELETE FROM tree WHERE changed = 1 AND active = 1 AND dirty = 1;
	UPDATE tree SET active = 1, dirty = 0, changed = 0 WHERE changed = 1;

-- C3. int cmdbc_discard (o)
	DELETE FROM tree WHERE changed = 1 AND active = 0;
	UPDATE tree SET dirty = 0, changed = 0 WHERE changed = 1;

-- Mx. void cmdbc_mark_v2 (o, id)
	WITH RECURSIVE list (id, parent) AS (
		SELECT id, parent FROM tree WHERE id = $id
		UNION ALL
		SELECT tree.id, tree.parent FROM tree, list
		WHERE tree.id = list.parent
	)
	UPDATE tree SET changed = 1 WHERE id = (SELECT id FROM list);

-- Dx. int cmdbc_delete_v2 (o, id)
	WITH RECURSIVE list (id) AS (
		SELECT id FROM tree WHERE id = $id AND active = 1
		UNION ALL
		SELECT tree.id FROM tree, list WHERE tree.parent = list.id
	)
	UPDATE tree SET dirty = 1, changed = 1 WHERE id = (SELECT id FROM list);

	WITH RECURSIVE list (id) AS (
		SELECT id FROM tree WHERE id = $id AND active = 0
		UNION ALL
		SELECT tree.id FROM tree, list WHERE tree.parent = list.id
	)
	DELETE FROM tree WHERE id = (SELECT id FROM list);

