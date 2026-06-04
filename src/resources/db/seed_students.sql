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
