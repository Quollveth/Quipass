#!/usr/bin/python3

def replace_between_tokens(input_file, token1, token2, replacement_file):
    "replaces all content between tokens with the contents of another file"
    with open(input_file, 'r+') as f:
        content = f.read()

    start_index = content.find(token1)
    end_index = content.find(token2)

    if start_index == -1 or end_index == -1:
        print("error")
        return

    with open(replacement_file, 'r') as f:
        replacement_content = f.read()

    new_content = content[:start_index + len(token1)] + replacement_content + content[end_index:]
    
    with open(input_file, 'w') as f:
        f.write(new_content)

if __name__ == "__main__":
    input_file = "src/main.cpp"
    token1 = "html("
    token2 = ")html"
    replacement_file = "ui-manager.html"

    replace_between_tokens(input_file, token1, token2, replacement_file)
    print(f"Updated ${input_file} with contents from ${replacement_file}")

