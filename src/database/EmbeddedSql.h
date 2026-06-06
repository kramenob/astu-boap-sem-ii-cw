#pragma once

namespace EmbeddedSql {

inline constexpr const char* students = R"SQL(
DROP TABLE IF EXISTS students;

CREATE TABLE students (
    id         INTEGER PRIMARY KEY AUTOINCREMENT,
    last_name  TEXT,
    first_name TEXT,
    patronymic TEXT,
    groupname  TEXT,
    gender     TEXT,
    department TEXT,
    email      TEXT,
    phone      TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

INSERT INTO students (
	last_name, first_name, patronymic, groupname, gender, department, email, phone
)

VALUES
( 'Фамилия', 'Имя', 'Отчество', 'ГРПП-01', 'М',   'АСОиУ', 'e@mail.com', '+79998887766' ),
( '...',     '...', '...',      '...',     '...', '...',   '...',        '...'          );
)SQL";

inline constexpr const char* teachers = R"SQL(
DROP TABLE IF EXISTS teachers;

CREATE TABLE teachers (
    id         INTEGER PRIMARY KEY AUTOINCREMENT,
    last_name  TEXT,
    first_name TEXT,
    patronymic TEXT,
    gender     TEXT,
    email      TEXT,
    phone      TEXT,
    department TEXT,
    role       TEXT,
    level      TEXT,
    dashboard  TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

INSERT INTO teachers (
last_name, first_name, patronymic, gender, email, phone, department, role, level, dashboard
)
VALUES
( 'Фамилия', 'Имя', 'Отчество', 'М',   'e@mail.com', '+79998887766', 'АСОиУ', 'Преподаватель', 'Ассистент', 'гл.426' ),
( '...',     '...', '...',      '...', '...',        '...',          '...',   '...',           '...',       '...'    );
)SQL";

}