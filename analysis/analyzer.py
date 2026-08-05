import os
from collections import Counter

def analyze_file():
    folder = "datasets"

    if not os.path.exists(folder):
        print("\nDatasets folder not found.")
        return

    files = [f for f in os.listdir(folder) if f.endswith(".txt")]

    if not files:
        print("\nNo text files found in datasets folder.")
        return

    print("\nAvailable Files:")
    for i, file in enumerate(files, 1):
        print(f"{i}. {file}")

    try:
        choice = int(input("\nSelect a file: "))
        filename = files[choice - 1]
    except:
        print("Invalid selection.")
        return

    filepath = os.path.join(folder, filename)

    with open(filepath, "r", encoding="utf-8") as file:
        text = file.read()

    characters = len(text)
    words = len(text.split())
    lines = len(text.splitlines())
    unique_characters = len(set(text))

    letters = [c.lower() for c in text if c.isalpha()]
    frequency = Counter(letters)
    print(f"File               : {filename}")
    print(f"Characters         : {characters}")
    print(f"Words              : {words}")
    print(f"Lines              : {lines}")
    print(f"Unique Characters  : {unique_characters}")

    print("\nLetter Frequency")
    for letter in "abcdefghijklmnopqrstuvwxyz":
        print(f"{letter} : {frequency.get(letter, 0)}")
