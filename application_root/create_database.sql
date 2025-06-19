CREATE TABLE user
(
    id           INTEGER PRIMARY KEY AUTOINCREMENT,
    email        TEXT,
    display_name TEXT,
    password     TEXT
);

CREATE TABLE quest
(
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    name        TEXT,
    description TEXT,
    parent      INTEGER,
    owner       INTEGER,
    editor      INTEGER,
    FOREIGN KEY (parent) REFERENCES quest(id) ON DELETE CASCADE,
    FOREIGN KEY (owner) REFERENCES user(id),
    FOREIGN KEY (editor) REFERENCES user(id)
);

-- delete all subQuests when parent quest is deleted
CREATE TRIGGER parent_delete AFTER DELETE ON quest BEGIN
    DELETE FROM quest WHERE parent = OLD.id;
END;
