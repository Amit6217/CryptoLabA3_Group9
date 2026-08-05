import sys
from datetime import datetime
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

    
LOG_FILE = "logs/activity.log"

def log_activity(option):
    with open(LOG_FILE, "a") as file:
        current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        file.write(f"{current_time} - {option}\n")


def display_menu():
    """Displays the main CLI menu for CryptoLabX."""
    print("\n" + "=" * 40)
    print("        CryptoLabX - Main Menu        ")
    print("=" * 40)
    print("1. Encrypt")
    print("2. Decrypt")
    print("3. Attack")
    print("4. Analyze")
    print("5. Exit")
    print("=" * 40)

def main():
    while True:
        display_menu()
        choice = input("Enter your choice (1-5): ").strip()
        if choice == '1':
            log_activity("Encrypt")
            print("\n[+] Encrypt module: Coming Soon!")
        elif choice == '2':
            log_activity("Decrypt")
            print("\n[+] Decrypt module: Coming Soon!")
        elif choice == '3':
            log_activity("Attack")
            print("\n[+] Attack module: Coming Soon!")
        elif choice == '4':
           log_activity("Analyze")
           analyze_file()
        elif choice == '5':
            log_activity("Exit")
            print("\nExiting CryptoLabX. Goodbye!")
            sys.exit(0)
        else:
            print("\n[!] Invalid choice! Please select a valid option (1-5).")

if __name__ == "__main__":
    main()
