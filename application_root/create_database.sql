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
    title       TEXT,
    description TEXT,
    status      TEXT,
    parent      INTEGER,
    owner       INTEGER,
    editor      INTEGER,
    FOREIGN KEY (parent) REFERENCES quest(id) ON DELETE CASCADE,
    FOREIGN KEY (owner) REFERENCES user(id),
    FOREIGN KEY (editor) REFERENCES user(id)
);