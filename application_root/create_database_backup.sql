create table user
(
    id           INTEGER PRIMARY KEY AUTOINCREMENT,
    email        TEXT,
    display_name TEXT,
    password     TEXT
);
--insert into user(email, display_name)
--VALUES ('sidequest_root@hs-aalen.de', 'Sidequest Root User', '');