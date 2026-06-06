-- uppercase.lua
-- Transform content inside :::uppercase ... ::: to uppercase

function Div(el)
  -- Check if the div has class "uppercase"
  if el.classes:includes("uppercase") then
    local new_content = {}
    for _, item in ipairs(el.content) do
      -- Only uppercase Str elements, keep formatting (like strong/bold)
      if item.t == "Str" then
        table.insert(new_content, pandoc.Str(string.upper(item.text)))
      elseif item.t == "Space" then
        table.insert(new_content, pandoc.Space())
      else
        -- Keep other elements recursively
        table.insert(new_content, item)
      end
    end
    return pandoc.Div(new_content)
  end
end