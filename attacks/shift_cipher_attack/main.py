from src.shift_cipher import encrypt, decrypt

text = "HELLO WORLD"
key = 3

cipher = encrypt(text, key)
print("Cipher:", cipher)

print("Plain:", decrypt(cipher, key))