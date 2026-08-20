import sys
import os

sys.path.append(os.path.join(os.path.dirname(__file__), "src"))
from src.shift_cipher import decrypt,encrypt
from src.brute_force_dictionary import load_words,dictionary_attack

text = "HELLO WORLD"
key = 3

cipher = encrypt(text, key)
print("Cipher:", cipher)

print("Plain:", decrypt(cipher, key))

print("brute-force and dictionary scoring.")
words = load_words("dictionary/english_words.txt")
dict_key, dict_text, dict_score = dictionary_attack(cipher, words)
print("\nDictionary Attack Result")
print("Predicted Key :", dict_key)
print("Plaintext     :", dict_text)
print("Score         :", dict_score)
if dict_key == key:
    print("Dictionary Attack: CORRECT")
else:
    print("Dictionary Attack: WRONG")
