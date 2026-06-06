#!/usr/bin/env bash
# Generate script for assembling DOCX course project
# Pipeline: Markdown -> Pandoc -> DOCX body -> Python merge with base DOCX
set -e

# Resolve script and project root directories
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/../../" && pwd)"

# Load project variables from docs/config/.env
ENV_FILE="$PROJECT_DIR/docs/config/.env"
if [ -f "$ENV_FILE" ]; then
    set -a
    . "$ENV_FILE"
    set +a
fi

# Ensure required tools are available
command -v python3 >/dev/null 2>&1 || { echo "python3 not found"; exit 1; }

# Define output directory and main DOCX assets
OUTPUT_DIR="$PROJECT_DIR/output"
# Base DOCX contains title page and formatting base
BASE_DOC="$PROJECT_DIR/docs/config/templates/base.docx"
# TEMP_BODY_DOC is intermediate Pandoc-generated document
TEMP_BODY_DOC="$OUTPUT_DIR/project.body.docx"

if [ ! -f "$BASE_DOC" ]; then
    echo "Base DOCX not found: $BASE_DOC"
    exit 1
fi


mkdir -p "$OUTPUT_DIR"

# Use isolated temporary directory per run to avoid race conditions between executions
TMP_MD_DIR="$(mktemp -d "$OUTPUT_DIR/.md-expanded.XXXXXX")"
# Cleanup temporary markdown directory on exit
trap 'rm -rf "$TMP_MD_DIR"' EXIT

# Collect all Markdown files in deterministic order (stable across runs)
FILES_ARR=()

while IFS= read -r src; do
    [ -z "$src" ] && continue

    dst="$TMP_MD_DIR/$(basename "$src")"
    envsubst < "$src" > "$dst"
    FILES_ARR+=("$dst")
done < <(
    find "$PROJECT_DIR/docs/main/project.d/" \
        -maxdepth 1 -type f -name "*.md" | sort -V
)

if [ ${#FILES_ARR[@]} -eq 0 ]; then
    echo "No Markdown files found in docs/main/project.d/"
    exit 1
fi

OUTPUT_FILE="$OUTPUT_DIR/project.body.docx"

# Convert Markdown files into DOCX body using Pandoc
pandoc "${FILES_ARR[@]}" \
    --from markdown \
    --toc \
	--toc-depth=3 \
	--metadata toc-title="Оглавление" \
    --standalone \
    --output "$OUTPUT_FILE" \
    --resource-path="$PROJECT_DIR/docs/main/project.d:." \
    --lua-filter="$SCRIPT_DIR/lua-filters/uppercase.lua" \
    --lua-filter="$SCRIPT_DIR/lua-filters/remove-hr.lua"

# Create and prepare Python virtual environment for docx merge step
VENV_DIR="$PROJECT_DIR/scripts/docs/.venv"
REQ_FILE="$PROJECT_DIR/scripts/docs/requirements"

if [ ! -d "$VENV_DIR" ]; then
    python3 -m venv "$VENV_DIR"
fi

# use venv python directly (avoid system pip / PEP 668 issues)
VENV_PY="$VENV_DIR/bin/python"

# Install required Python dependencies inside virtual environment
$VENV_PY -m pip install --upgrade pip > /dev/null
$VENV_PY -m pip install -r "$REQ_FILE"

# Merge base DOCX (title pages) with generated body DOCX
$VENV_PY "$SCRIPT_DIR/merge.py" \
    "$ENV_FILE" \
    "$BASE_DOC" \
    "$OUTPUT_FILE" \
    "$OUTPUT_DIR/project.docx"

# Output success message
if [ $? -eq 0 ]; then
    echo "Generated project DOCX: $OUTPUT_DIR/project.docx"
else
    echo "Failed to generate DOCX"
    exit 1
fi