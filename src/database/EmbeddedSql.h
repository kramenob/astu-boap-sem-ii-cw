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
    role       TEXT
);

INSERT INTO students (
	last_name, first_name, patronymic, groupname, gender, department, email, phone, role
)

VALUES
( 'Петров',   'Иван',    'Сергеевич',  'Группа 1', 'М', 'Факультет 1', 'ivan@student.ru',   '+79990000001', 'Student' ),
( 'Соколова', 'Анна',    'Игоревна',   'Группа 2', 'Ж', 'Факультет 2', 'anna@student.ru',   '+79990000002', 'Student' ),
( 'Кузнецов', 'Дмитрий', 'Алексеевич', 'Группа 3', 'М', 'Факультет 3', 'dmitry@student.ru', '+79990000003', 'Student' );
)SQL";

inline constexpr const char* teachers = R"SQL(
DROP TABLE IF EXISTS teachers;

CREATE TABLE teachers (
    id         INTEGER PRIMARY KEY AUTOINCREMENT,
    last_name  TEXT,
    first_name TEXT,
    patronymic TEXT,
    photo      TEXT,
    age        INTEGER,
    gender     TEXT,
    city       TEXT,
    address    TEXT,
    email      TEXT,
    phone      TEXT,
    role       TEXT,
    department TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

INSERT INTO teachers (
last_name, first_name, patronymic, photo, age, gender, city, address, email, phone, role, department
)
VALUES
('Смирнов', 'Алексей', 'Викторович', 'https://thispersondoesnotexist.com/', 35, 'М', 'Санкт-Петербург', 'Невский пр. 10', 'asmirnov@company.ru', '+79991110001', 'Engineer',   'IT'      ),
('Иванова', 'Мария',   'Петровна',   'https://thispersondoesnotexist.com/', 29, 'Ж', 'Москва',          'ул. Тверская 7', 'mivanova@company.ru', '+79991110002', 'Accountant', 'Finance' ),
('Орлов',   'Егор',    'Андреевич',  'https://thispersondoesnotexist.com/', 41, 'М', 'Екатеринбург',    'ул. Лесная 15',  'eorlov@company.ru',   '+79991110003', 'Manager',    'Project' );

)SQL";

}