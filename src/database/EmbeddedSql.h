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
( 'Алферов', 'Алексей', 'Валерьевич', 'ДИНРБ-11/1', 'М', 'АСОиУ', 'svet-dusha77@gmail.com', '+79275421548'),
( 'Зубайраев', 'Ибрагим', 'Хамитович', 'ДИНРБ-11/1', 'М', 'АСОиУ', 'ogon-serdce88@mail.ru', '+79377779854'),
( 'Лёвкин', 'Владимир', 'Борисович', 'ДИНРБ-11/1', 'М', 'АСОиУ', 'mir-haos99@yandex.ru', '+79966547999'),
( 'Лихман', 'Камилла', 'Алексеевна', 'ДИНРБ-11/1', 'Ж', 'АСОиУ', 'veter-tma21@gmail.com', '+79053567553'),
( 'Лосенков', 'Георгий', 'Олегович', 'ДИНРБ-11/1', 'М', 'АСОиУ', 'razum-bezumie34@mail.ru', '+79624456676'),
( 'Марданов', 'Фаик', 'Вугарович', 'ДИНРБ-11/1', 'М', 'АСОиУ', 'noch-den56@yandex.ru', '+79177632345'),
( 'Мошков', 'Константин', 'Олегович', 'ДИНРБ-11/1', 'М', 'АСОиУ', 'krylya-neba11@gmail.com', '+79483456767'),
( 'Новоселов', 'Егор', 'Юрьевич', 'ДИНРБ-11/1', 'М', 'АСОиУ', 'tsvet-molniya22@mail.ru', '+79518765544'),
( 'Петров', 'Тимофей', 'Александрович', 'ДИНРБ-11/1', 'М', 'АСОиУ', 'led-ognya33@yandex.ru', '+79203557268'),
( 'Полухин', 'Михаил', 'Алексеевич', 'ДИНРБ-11/1', 'М', 'АСОиУ', 'ten-sveta44@gmail.com', '+79334845642'),
( 'Сахипов', 'Руслан', 'Кайратович', 'ДИНРБ-11/1', 'М', 'АСОиУ', 'zvezda-pyli55@mail.ru', '+79885567223'),
( 'Хайдаров', 'Магомед', 'Ризванович', 'ДИНРБ-11/1', 'М', 'АСОиУ', 'grom-tishina66@yandex.ru', '+79743357733'),
( 'Абделхалем Мохамед', 'Ганна', 'Абделнассер Рашед', 'ДИНРБ-11/2', 'Ж', 'АСОиУ', 'reka-ognya77@gmail.com', '+79012899766'),
( 'Бол', 'Бол', 'Яй Тап', 'ДИНРБ-11/2', 'М', 'АСОиУ', 'luna-solnca88@mail.ru', '+79691156145'),
( 'Болотин', 'Максим', 'Владимирович', 'ДИНРБ-11/2', 'М', 'АСОиУ', 'volk-dusha99@yandex.ru', '+79127876543'),
( 'Гайнулин', 'Камиль', 'Ильнурович', 'ДИНРБ-11/2', 'М', 'АСОиУ', 'plamya-serdca10@gmail.com', '+79251267886'),
( 'Жилин', 'Даниил', 'Олегович', 'ДИНРБ-11/2', 'М', 'АСОиУ', 'more-vetra12@mail.ru', '+79301256725'),
( 'Илюшенко', 'Виктория', 'Александровна', 'ДИНРБ-11/2', 'Ж', 'АСОиУ', 'les-tymy13@yandex.ru', '+79917452345'),
( 'Колани', 'Лалле', '', 'ДИНРБ-11/2', 'М', 'АСОиУ', 'kamen-neba14@gmail.com', '+79444855762'),
( 'Мукашев', 'Эльнур', 'Гайнурович', 'ДИНРБ-11/2', 'М', 'АСОиУ', 'put-zvezd15@mail.ru', '+79577975235'),
( 'Мухаджинов', 'Ролан', 'Ренатович', 'ДИНРБ-11/2', 'М', 'АСОиУ', 'volna-haosa16@yandex.ru', '+79603783525'),
( 'Нгуен', 'Хоанг', 'Чонг Хай', 'ДИНРБ-11/2', 'М', 'АСОиУ', 'angel-tymy17@gmail.com', '+79223689351'),
( 'Ткачев', 'Михаил', 'Алексеевич', 'ДИНРБ-11/2', 'М', 'АСОиУ', 'demon-sveta18@mail.ru', '+79394237235');
-- ( 'Фамилия', 'Имя', 'Отчество', 'ГРПП-01', 'М',   'АСОиУ', 'e@mail.com', '+79998887766' ),
-- ( '...',     '...', '...',      '...',     '...', '...',   '...',        '...'          );
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