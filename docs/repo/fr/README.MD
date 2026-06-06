# MarkForge

[English](../../../README) | [Русский](../ru/README.MD) | [العربية](../ar/README.MD) | [Français](README.MD)

Application en ligne de commande multiplateforme pour la génération de documents à partir d'une base de données.

Plateformes prises en charge :
- Windows x64
- Linux x64
- macOS (Intel et Apple Silicon)

---

## Installation

### Télécharger une version prête à l'emploi

Téléchargez la dernière version depuis la page GitHub Releases du projet.

### Linux / macOS

Rendez le binaire exécutable :

```bash
chmod +x markforge
```

Exécutez ensuite :

```bash
./markforge --help
```

### Windows

Exécutez depuis PowerShell ou l'invite de commandes :

```powershell
.\markforge.exe --help
```

---

## Démarrage rapide

Créer une base de données de démonstration :

```bash
markforge db --init --template students
```

Lister les tables disponibles :

```bash
markforge db --list
```

Afficher les enregistrements :

```bash
markforge data --show students
```

Générer des documents :

```bash
markforge generate --source students --template invitation
```

---

## Référence des commandes

### Commandes générales

```bash
markforge --help
markforge --version
```

### Commandes de base de données

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

### Commandes de données

```bash
markforge data --show
markforge data --show <table_name>
```

### Commandes de modèles

```bash
markforge template --list
markforge template --show <template_name>
markforge template --add <path/to/template.docx> -as <template_name>
markforge template --remove <template_name>
```

### Génération de documents

```bash
markforge generate --source <table_name> --template <template_name>
markforge generate --source <table_name> --template <template_name> --output <path/to/output>
```

---

## Formats de modèles pris en charge

Actuellement pris en charge :

- TXT
- Markdown (.md)
- DOCX
- XLS
- XLSX

Prise en charge prévue :

- PDF import
- ODT
- HTML

---

## Exemple de workflow

```bash
markforge db --init --template students
markforge db --list
markforge data --show students
markforge template --list
markforge generate --source students --template invitation
```

---

## Licence

Projet académique réalisé dans le cadre d'un cours.

---

Traduit pour mes amis francophones.
