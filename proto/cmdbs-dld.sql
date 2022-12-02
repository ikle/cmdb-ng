--
-- Template tree
--

-- T0. static int cmdbs_tree_init (o)
	CREATE TABLE IF NOT EXISTS tree (
		id	INTEGER PRIMARY KEY,	-- id > 0
		parent	INTEGER NOT NULL,	-- 0 for root
		kind	INTEGER NOT NULL,
		label	TEXT    NOT NULL,
		info	TEXT    NOT NULL,	-- short description

		UNIQUE (parent, label)
	);

	CREATE INDEX IF NOT EXISTS 'tree-parent' ON tree (parent);

	INSERT OR IGNORE INTO tree (id, parent, kind, label, info)
	VALUES (1, 0, 0, 'root', 'Configuration Template');

	INSERT OR IGNORE INTO tree (id, parent, kind, label, info)
	VALUES (2, 0, 0, 'class', 'Configuration Classe');

-- T4. static long cmdbs_tree_search (o, parent, label)
	SELECT id FROM tree WHERE parent = $parent AND label = $label;

-- T1. long cmdbs_node_add (o, parent, kind, label, info)
	INSERT INTO tree (parent, kind, label, info)
	VALUES ($parent, $kind, $label, $info);

-- T2. static struct cmdbs_node *cmdbs_node_lookup_nr (o, parent, label)
	SELECT * FROM tree WHERE parent = $parent AND label = $label;

--     struct cmdbs_node *cmdbs_node_lookup (o, parent, label)
--	cmdbs_node_lookup_nr (o, parent, label) or
--	any cmdbs_node_lookup (o, link, label) for each class (o, parent)

-- T3. int  cmdbs_node_first (struct cmdbs_node *o, struct cmdbs *s, parent)
--     int  cmdbs_node_next  (struct cmdbs_node *o)
--     void cmdbs_node_done  (struct cmdbs_node *o)
	SELECT * FROM tree WHERE parent = $parent ORDER BY label ASC;

--
-- Class links
--

-- C0. static int cmdbs_class_init (o)
	CREATE TABLE IF NOT EXISTS classes (
		node	INTEGER NOT NULL,
		name	TEXT NOT NULL,
		root	INTEGER,		-- tree id for present classes

		UNIQUE (node, name)
	);

	CREATE INDEX IF NOT EXISTS 'classes-node' ON classes (node);

-- C1. int cmdbs_class_add (o, node, name)
	INSERT INTO classes (node, name) VALUES ($node, $name);

-- C2. int cmdbs_class_link (o)
	UPDATE classes SET root = (
		SELECT id FROM tree
		WHERE tree.parent = 2 AND tree.label = classes.name
	);

-- C3. int  cmdbs_class_first (struct cmdbs_class *o, struct cmdbs *s, node)
--     int  cmdbs_class_next  (struct cmdbs_class *o)
--     void cmdbs_class_done  (struct cmdbs_class *o)
	SELECT name, root FROM classes WHERE node = $node AND root IS NOT NULL;

--
-- Values
--

-- V0. static int cmdbs_value_init (o)
	CREATE TABLE IF NOT EXISTS 'values' (
		node	INTEGER NOT NULL,
		label	TEXT    NOT NULL,
		info	TEXT    NOT NULL,

		UNIQUE (node, label)
	);

	CREATE INDEX IF NOT EXISTS 'values-node' ON values (node);

-- V1. int cmdbs_value_add (o, node, label, info)
	INSERT INTO 'values' (node, label, info) VALUES ($node, $label, $info);

-- V2. static int cmdbs_check_value_nr (o, node, value)
	SELECT COUNT (1) FROM 'values'
	WHERE node = $node AND label = $value LIMIT 1;

-- V3. int  cmdbs_value_first (struct cmdbs_value *o, struct cmdbs *s, node)
--     int  cmdbs_value_next  (struct cmdbs_value *o)
--     void cmdbs_value_done  (struct cmdbs_value *o)
	SELECT label, info FROM 'values' WHERE node = $node;

--
-- Numbers
--

-- N0. static int cmdbs_number_init (o)
	CREATE TABLE IF NOT EXISTS numbers (
		node	INTEGER NOT NULL,
		lo	INTEGER NOT NULL,
		hi	INTEGER NOT NULL,

		UNIQUE (node, lo, hi)
	);

	CREATE INDEX IF NOT EXISTS 'numbers-node' ON numbers (node);

-- N1. int cmdbs_number_add (o, node, lo, hi)
	INSERT INTO numbers (node, lo, hi) VALUES ($node, $lo, $hi);

-- N2. static int cmdbs_check_number_nr (o, node, value)
	SELECT COUNT (1) FROM numbers
	WHERE node = $node AND lo >= $value AND $value <= hi LIMIT 1;

-- N3. int  cmdbs_number_first (struct cmdbs_number *o, struct cmdbs *s, node)
--     int  cmdbs_number_next  (struct cmdbs_number *o)
--     void cmdbs_number_done  (struct cmdbs_number *o)
	SELECT lo, hi FROM numbers WHERE node = $node;

--
-- Matches, match is a custom function
--

-- M0. static int cmdbs_match_init (o)
	CREATE TABLE IF NOT EXISTS matches (
		node	INTEGER NOT NULL,
		name	TEXT NOT NULL,
		re	TEXT,

		UNIQUE (node, name)
	);

	CREATE INDEX IF NOT EXISTS 'matches-node' ON matches (node);

-- M1. int cmdbs_match_add (o, node, name, re)
	INSERT INTO matches (node, name, re) VALUES ($node, $name, $re);

-- M2. static int cmdbs_check_match_nr (o, node, value)
	SELECT COUNT (1) FROM matchers
	WHERE node = $node AND match ($value, name, re) LIMIT 1;

-- M3. int  cmdbs_match_first (struct cmdbs_match *o, struct cmdbs *s, node)
--     int  cmdbs_match_next  (struct cmdbs_match *o)
--     void cmdbs_match_done  (struct cmdbs_match *o)
	SELECT name, re FROM matchers WHERE node = $node;

--
-- Syntax Checher
--

-- S0. static int cmdbs_check_value (o, node, value)
--	cmdbs_check_value_nr (o, node, value) or
--	any cmdbs_check_value (o, link, value) for each class (o, node)

-- S1. static int cmdbs_check_number (o, node, value)
--	cmdbs_check_number_nr (o, node, value) or
--	any cmdbs_check_number (o, link, value) for each class (o, node)

-- S2. static int cmdbs_check_match (o, node, value)
--	cmdbs_check_match_nr (o, node, value) or
--	any cmdbs_check_match (o, link, value) for each class (o, node)

-- S3. int cmdbs_check (o, node, value)
--	cmdbs_check_value  (o, node, value) or
--	cmdbs_check_number (o, node, value) or
--	cmdbs_check_match  (o, node, value)

--
-- Dahabase handle
--

-- H0. static int cmdbs_init (o)
--	cmdbs_tree_init   (o) and
--	cmdbs_class_init  (o) and
--	cmdbs_value_init  (o) and
--	cmdbs_number_init (o) and
--	cmdbs_match_init  (o)

-- H1. struct cmdbs *cmdbs_writer (path)
--	open database for writing
--	cmdbs_init (o)

-- H2. struct cmdbs *cmdbs_reader (path)
--	open database for reading

-- H3. void cmdbs_close (o)

--
-- Modules
--

-- cmdbs-writer		: T0, C0, V0, N0, M0, H0-H1
-- cmdbs-reader		: H2
-- cmdbs-close		: H3

-- cmdbs-adder		: T1, C1-C2, V1, N1, M1
-- cmdbs-lookup		: T2
-- cmdbs-check		: V2, N2, M2, S0-S3

-- cmdbs-scan-tree	: T3
-- cmdbs-scan-class	: C3
-- cmdbs-scan-value	: V3
-- cmdbs-scan-number	: N3
-- cmdbs-scan-match	: M3

