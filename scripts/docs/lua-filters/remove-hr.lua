-- @file remove-hr.lua
-- @brief Pandoc Lua filter that removes horizontal rules from Markdown output.

-- ---------------- Filter Implementation ----------------
function HorizontalRule(el)
  return {}
end