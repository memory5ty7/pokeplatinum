import os

# Replace this with the root directory path
root_dir = "C:Users/amaur/Downloads/pokemon"

# Traverse through all subdirectories
for dirpath, dirnames, filenames in os.walk(root_dir):
    for filename in filenames:
        # Check if the file is not named 'data.json'
        if filename != "data.json":
            file_path = os.path.join(dirpath, filename)
            os.remove(file_path)
            print(f"Deleted: {file_path}")