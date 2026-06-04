DROP TABLE IF EXISTS employees;

CREATE TABLE employees (
    id         INTEGER PRIMARY KEY AUTOINCREMENT,
    last_name  TEXT,
    first_name TEXT,
    patronymic TEXT,
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

INSERT INTO employees (
last_name, first_name, patronymic, age, gender, city, address, email, phone, role, department
)
VALUES
('Смирнов', 'Алексей', 'Викторович', 35, 'М', 'Санкт-Петербург', 'Невский пр. 10', 'asmirnov@company.ru', '+79991110001', 'Engineer',   'IT'      ),
('Иванова', 'Мария',   'Петровна',   29, 'Ж', 'Москва',          'ул. Тверская 7', 'mivanova@company.ru', '+79991110002', 'Accountant', 'Finance' ),
('Орлов',   'Егор',    'Андреевич',  41, 'М', 'Екатеринбург',    'ул. Лесная 15',  'eorlov@company.ru',   '+79991110003', 'Manager',    'Project' );
