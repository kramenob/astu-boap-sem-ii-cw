# MarkForge

[English](../../../README.MD) | [Русский](README.MD) | [العربية](../ar/README.MD) | [Français](../fr/README.MD)

## Установка

### Загрузка готовой сборки

Вы можете скачать готовую сборку для вашей операционной системы с [GitHub Releases](https://github.com/kramenob/markforge/releases).

### Быстрый старт

После установки вы можете быстро проверить работу MarkForge, выполнив:

```bash
markforge --help
```

## Справочник команд

### Общие команды

- `markforge --help` — показать справку по командам.
- `markforge --version` — показать версию программы.

### Команды базы данных

- `markforge db init` — инициализировать базу данных.
- `markforge db migrate` — применить миграции к базе данных.

### Команды данных

- `markforge data import <file>` — импортировать данные из файла.
- `markforge data export <file>` — экспортировать данные в файл.

### Команды шаблонов

- `markforge template add <template>` — добавить новый шаблон.
- `markforge template list` — показать список шаблонов.

### Генерация документов

- `markforge generate <template> <output>` — сгенерировать документ на основе шаблона.

## Поддерживаемые форматы шаблонов

MarkForge поддерживает следующие форматы шаблонов:

- Markdown
- HTML
- LaTeX

## Пример рабочего процесса

1. Инициализируйте базу данных:

```bash
markforge db init
```

2. Импортируйте данные:

```bash
markforge data import data.csv
```

3. Добавьте шаблон:

```bash
markforge template add report.md
```

4. Сгенерируйте документ:

```bash
markforge generate report.md output.pdf
```

## Лицензия

MarkForge распространяется под лицензией MIT. Подробнее см. в файле LICENSE.

---

Переведено для моих русскоговорящих друзей.
