from shift_cipher import decrypt

def load_words(filename):
    words = set()
    with open(filename, "r") as file:
        for line in file:
            word = line.strip().lower()
            if word:
                words.add(word)
    return words


def dictionary_score(text, words):
    score = 0
    for word in text.lower().split():
        word = word.strip(".,!?;:\"'")
        if word in words:
            score += 1
    return score

def dictionary_attack(ciphertext, words):
    best_key = 0
    best_text = ""
    best_score = -1
    for key in range(26):
        plaintext = decrypt(ciphertext, key)
        score = dictionary_score(plaintext, words)
        print("Key:", key, "| Score:", score, "|", plaintext)
        if score > best_score:
            best_score = score
            best_key = key
            best_text = plaintext
    return best_key, best_text, best_score