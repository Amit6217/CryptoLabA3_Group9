# CryptoLabA3_Group9

## Project Description
CryptoLabA3_Group9 is our custom cryptanalysis toolkit developed for the Cryptography Laboratory course. Throughout this semester, we are building this project from the ground up to analyze, encrypt, and decipher various cryptographic algorithms. Currently, we have established the foundational architecture, set up our version control workflow with Git, and implemented a basic command-line interface with file analysis capabilities.

## Instructor
- Dr. Meenakshi Tripathi

## Team Members
- **2024UCP1562** - [AMIT KUMAR](https://github.com/Amit6217)
- **2024UCP1717** - [GUTTULA POOJA](https://github.com/GUTTULAPOOJA)


## Folder Structure
```text
CryptoLabA3_Group9/
├── classical/
├── attacks/
├── math/
├── modern/
├── analysis/
├── datasets/
├── outputs/
├── docs/
├── tests/
├── utils/
├── main.py
├── README.md
└── requirements.txt
```

## How to Use

1. **Clone the repository:**
   ```bash
   git clone https://github.com/Amit6217/CryptoLabA3_Group9.git
   cd CryptoLabA3_Group9
   ```
2. **Run the toolkit:**
   ```bash
   python main.py
   ```
3. **Use the Menu:**
   - The interactive CLI will display a menu. 
   - Enter a number (`1-5`) to select an option (e.g., `4` to Analyze a text file from the `datasets/` folder).
   - All activities are logged automatically in `logs/activity.log`.

## Progress
- **Week 1**: Established project foundation, folder structure, and Git repository. Implemented an interactive CLI menu with logging capabilities, and developed a dedicated file analysis module to process and extract text metrics from datasets.

## Future Modules
In upcoming weeks, we will be implementing:
- **Classical Ciphers**: Caesar, Vigenère, Playfair, etc.
- **Modern Ciphers**: DES, AES, RSA, etc.
- **Cryptanalysis Attacks**: Brute force, frequency analysis, known-plaintext attacks, etc.
- **Mathematical Utilities**: Modular arithmetic, prime generation, extended Euclidean algorithm, etc.