<div dir="rtl">

# MarkForge

[English](../../../README.MD) | [Русский](../ru/README.MD) | [العربية](README.MD) | [Français](../fr/README.MD)

تطبيق سطر أوامر متعدد المنصات لإنشاء المستندات اعتماداً على قواعد البيانات.

المنصات المدعومة:
- Windows x64
- Linux x64
- macOS (Intel و Apple Silicon)

---

## التثبيت

### تنزيل إصدار جاهز

قم بتنزيل أحدث إصدار من [صفحة GitHub Releases الخاصة بالمشروع](/kramenob/markforge/releases).

### Linux / macOS

اجعل الملف التنفيذي قابلاً للتنفيذ:

```bash
chmod +x markforge
```

ثم شغّله:

```bash
./markforge --help
```

### Windows

شغّل البرنامج من PowerShell أو Command Prompt:

```powershell
.\markforge.exe --help
```

---

## البدء السريع

إنشاء قاعدة بيانات تجريبية:

```bash
markforge db --init --template students
```

عرض الجداول المتاحة:

```bash
markforge db --list
```

عرض السجلات:

```bash
markforge data --show students
```

إنشاء المستندات:

```bash
markforge generate --source students --template invitation
```

---

## مرجع الأوامر

### الأوامر العامة

```bash
markforge --help
markforge --version
```

### أوامر قاعدة البيانات

```bash
markforge db --init
markforge db --init --templates
markforge db --init --template employees
markforge db --init --template students
markforge db --drop <table_name>
markforge db --drop --all
markforge db --list
markforge db --list --all
markforge db --schema-show <table_name>
```

### أوامر البيانات

```bash
markforge data --show
markforge data --show <table_name>
```

### أوامر القوالب

```bash
markforge template --list
markforge template --show <template_name>
markforge template --add <path/to/template.docx> -as <template_name>
markforge template --remove <template_name>
```

### إنشاء المستندات

```bash
markforge generate --source <table_name> --template <template_name>
markforge generate --source <table_name> --template <template_name> --output <path/to/output>
```

---

## تنسيقات القوالب المدعومة

مدعوم حالياً:

- TXT
- Markdown (.md)
- DOCX
- XLS
- XLSX

مخطط دعمه مستقبلاً:

- PDF import
- ODT
- HTML

---

## مثال على سير العمل

```bash
markforge db --init --template students
markforge db --list
markforge data --show students
markforge template --list
markforge generate --source students --template invitation
```

---

## الترخيص

مشروع أكاديمي ضمن مقرر دراسي.

---

تمت الترجمة لأصدقائي الناطقين بالروسية.

</div>