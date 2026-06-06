# MarkForge

[English](README) | [Русский](docs/repo/ru/README.MD) | [العربية](docs/repo/ar/README.MD) | [Français](docs/repo/fr/README.MD)

Cross-platform command-line application for database-driven document generation.

Supported platforms:
- Windows x64
- Linux x64
- macOS (Intel and Apple Silicon)

---

## Installation

### Download a Release Build

Download the latest binary from the project's [GitHub Releases](/kramenob/markforge/releases) page.

### Linux / macOS

Make the binary executable:

```bash
chmod +x markforge
```

Run:

```bash
./markforge --help
```

### Windows

Run from PowerShell or Command Prompt:

```powershell
.\markforge.exe --help
```

---

## Quick Start

Create a demo database:

```bash
markforge db --init --template students
```

List available tables:

```bash
markforge db --list
```

Show records:

```bash
markforge data --show students
```

Generate documents:

```bash
markforge generate --source students --template invitation
```

---

# Command Reference

## General Commands

### Show Help

Display application description, version information, and available commands.

```bash
markforge --help
```

### Show Version

Display application version.

```bash
markforge --version
```

---

## Database Commands

### Initialize Database

Interactive initialization:

```bash
markforge db --init
```

Show available templates:

```bash
markforge db --init --templates
```

Create a database using a predefined template:

```bash
markforge db --init --template employees
```

```bash
markforge db --init --template students
```

### Remove Tables

Remove a table:

```bash
markforge db --drop <table_name>
```

Remove all tables:

```bash
markforge db --drop --all
```

### List Tables

User tables only:

```bash
markforge db --list
```

All tables:

```bash
markforge db --list --all
```

### Show Table Schema

```bash
markforge db --schema-show <table_name>
```

---

## Data Commands

### Show Records

Interactive mode:

```bash
markforge data --show
```

Show records from a specific table:

```bash
markforge data --show <table_name>
```

---

## Template Commands

### List Templates

```bash
markforge template --list
```

### Show Template

```bash
markforge template --show <template_name>
```

### Register Template

TXT, Markdown, and DOCX templates can be registered for later use.

```bash
markforge template --add <path/to/template.docx> -as <template_name>
```

### Remove Template

```bash
markforge template --remove <template_name>
```

---

## Document Generation

Generate documents using a table as the data source and a registered template.

### Generate for Selected Records

```bash
markforge generate --source <table_name> --template <template_name>
```

### Custom Output Directory

```bash
markforge generate --source <table_name> --template <template_name> --output <path/to/output>
```

---

## Supported Template Formats

Currently supported:

- TXT
- Markdown (.md)
- DOCX
- XLS
- XLSX

Planned support:

- PDF import
- ODT
- HTML

---

## Example Workflow

```bash
markforge db --init --template students
markforge db --list
markforge data --show students
markforge template --list
markforge generate --source students --template invitation
```

---

## License

Academic course work project.
