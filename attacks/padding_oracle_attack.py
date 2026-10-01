from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes


BLOCK_SIZE = 16


# ============================================================
# PKCS#7 PADDING
# ============================================================

def pkcs7_pad(data):
    padding_length = BLOCK_SIZE - (len(data) % BLOCK_SIZE)
    padding = bytes([padding_length]) * padding_length
    return data + padding


def pkcs7_unpad(data):
    if len(data) == 0:
        raise ValueError("Empty data")

    padding_length = data[-1]

    if padding_length < 1 or padding_length > BLOCK_SIZE:
        raise ValueError("Invalid padding")

    if data[-padding_length:] != bytes([padding_length]) * padding_length:
        raise ValueError("Invalid padding")

    return data[:-padding_length]


# ============================================================
# AES-CBC ENCRYPTION
# ============================================================

def encrypt_aes_cbc(plaintext, key, iv):
    padded = pkcs7_pad(plaintext)

    cipher = AES.new(key, AES.MODE_CBC, iv)
    ciphertext = cipher.encrypt(padded)

    return ciphertext


# ============================================================
# PADDING ORACLE
# ============================================================

class PaddingOracle:
    """
    Simulates a vulnerable server.

    The key is stored internally.
    The attacker NEVER receives the key.

    The oracle only tells the attacker whether
    the resulting plaintext has valid PKCS#7 padding.
    """

    def __init__(self, key):
        self.key = key
        self.query_count = 0

    def check(self, ciphertext, iv):
        self.query_count += 1

        try:
            cipher = AES.new(self.key, AES.MODE_CBC, iv)

            plaintext = cipher.decrypt(ciphertext)

            # The vulnerable application checks padding.
            pkcs7_unpad(plaintext)

            return True

        except (ValueError, KeyError):
            return False


# ============================================================
# PADDING ORACLE ATTACK
# ============================================================

def padding_oracle_attack(ciphertext, iv, oracle):
    """
    Recover plaintext without knowing the AES key.

    Inputs:
        ciphertext - encrypted message
        iv         - initialization vector
        oracle     - function that tells us whether padding is valid

    The key is NOT passed to this function.
    """

    blocks = [
        ciphertext[i:i + BLOCK_SIZE]
        for i in range(0, len(ciphertext), BLOCK_SIZE)
    ]

    recovered_plaintext = bytearray()

    # For every ciphertext block
    for block_number in range(len(blocks)):

        current_block = blocks[block_number]

        if block_number == 0:
            previous_block = iv
        else:
            previous_block = blocks[block_number - 1]

        # Bytes recovered for this plaintext block
        intermediate = bytearray(BLOCK_SIZE)

        plaintext_block = bytearray(BLOCK_SIZE)

        # Recover bytes from right to left
        for position in range(BLOCK_SIZE - 1, -1, -1):

            padding_value = BLOCK_SIZE - position

            found = False

            # Try all possible byte values
            for guess in range(256):

                modified_previous = bytearray(previous_block)

                # Force already recovered bytes to produce
                # the desired padding value.
                for j in range(position + 1, BLOCK_SIZE):
                    modified_previous[j] = (
                        previous_block[j]
                        ^ intermediate[j]
                        ^ padding_value
                    )

                # Try the current byte
                modified_previous[position] = (
                    previous_block[position]
                    ^ guess
                    ^ padding_value
                )

                modified_ciphertext = (
                    bytes(modified_previous) + current_block
                )

                # Ask the padding oracle
                if oracle.check(
                    modified_ciphertext,
                    bytes(modified_previous)
                    if False else b"\x00" * 16
                ):
                    pass

            # This loop is replaced below.
            # Kept here only for explanation.
            pass

    return bytes(recovered_plaintext)


# ============================================================
# CORRECT ATTACK IMPLEMENTATION
# ============================================================

def recover_block(previous_block, current_block, oracle):
    """
    Recover one plaintext block.

    Only previous_block, current_block and oracle
    are available.

    AES key is NOT available here.
    """

    intermediate = bytearray(BLOCK_SIZE)
    plaintext = bytearray(BLOCK_SIZE)

    for position in range(BLOCK_SIZE - 1, -1, -1):

        padding_value = BLOCK_SIZE - position

        for guess in range(256):

            modified_previous = bytearray(previous_block)

            # Modify bytes that have already been recovered.
            for j in range(position + 1, BLOCK_SIZE):
                modified_previous[j] = (
                    previous_block[j]
                    ^ intermediate[j]
                    ^ padding_value
                )

            # Try a candidate value for the current plaintext byte.
            modified_previous[position] = (
                previous_block[position]
                ^ guess
                ^ padding_value
            )

            test_ciphertext = bytes(modified_previous) + current_block

            # We need an oracle that can decrypt this two-block
            # ciphertext while using the original IV.
            if oracle(test_ciphertext):
                intermediate[position] = guess ^ padding_value

                plaintext[position] = (
                    intermediate[position]
                    ^ previous_block[position]
                )

                break

        else:
            raise RuntimeError(
                f"Could not recover byte at position {position}"
            )

    return bytes(plaintext)


def attack(ciphertext, iv, padding_oracle):
    """
    Complete padding oracle attack.

    padding_oracle(ciphertext, iv) must return:

        True  -> valid PKCS#7 padding
        False -> invalid PKCS#7 padding

    The AES key is never supplied here.
    """

    blocks = [
        ciphertext[i:i + BLOCK_SIZE]
        for i in range(0, len(ciphertext), BLOCK_SIZE)
    ]

    recovered = bytearray()

    previous = iv

    for current in blocks:

        plaintext_block = recover_block_with_iv(
            previous,
            current,
            iv,
            padding_oracle
        )

        recovered.extend(plaintext_block)

        previous = current

    return pkcs7_unpad(bytes(recovered))


def recover_block_with_iv(
    previous_block,
    current_block,
    original_iv,
    padding_oracle
):
    """
    Recover one plaintext block using the oracle.

    For the first block, the previous block is the IV.

    We construct:

        modified_previous || current_block

    and submit it to the oracle.
    """

    intermediate = bytearray(BLOCK_SIZE)
    plaintext = bytearray(BLOCK_SIZE)

    for position in range(BLOCK_SIZE - 1, -1, -1):

        padding_value = BLOCK_SIZE - position

        found = False

        for guess in range(256):

            modified_previous = bytearray(previous_block)

            # Force already recovered bytes to have
            # the required padding.
            for j in range(position + 1, BLOCK_SIZE):
                modified_previous[j] = (
                    previous_block[j]
                    ^ intermediate[j]
                    ^ padding_value
                )

            # Candidate for current byte
            modified_previous[position] = (
                previous_block[position]
                ^ guess
                ^ padding_value
            )

            test_ciphertext = bytes(modified_previous) + current_block

            # The oracle expects IV separately.
            # We use the original IV only when decrypting
            # the first ciphertext block.
            if padding_oracle(test_ciphertext, original_iv):
                intermediate[position] = guess ^ padding_value

                plaintext[position] = (
                    intermediate[position]
                    ^ previous_block[position]
                )

                found = True
                break

        if not found:
            raise RuntimeError(
                "Failed to recover byte "
                + str(position)
            )

    return bytes(plaintext)


# ============================================================
# DEMONSTRATION
# ============================================================

def main():

    # --------------------------------------------------------
    # SERVER SIDE
    # --------------------------------------------------------

    # This key belongs to the server.
    # The attack code NEVER receives it.
    secret_key = get_random_bytes(16)

    iv = get_random_bytes(16)

    original_message = (
        b"Padding Oracle Attack demonstration. "
        b"This plaintext is recovered without the key."
    )

    ciphertext = encrypt_aes_cbc(
        original_message,
        secret_key,
        iv
    )

    # Create vulnerable oracle
    oracle_object = PaddingOracle(secret_key)

    # --------------------------------------------------------
    # ATTACKER SIDE
    # --------------------------------------------------------

    def oracle(ciphertext_to_test, supplied_iv):

        return oracle_object.check(
            ciphertext_to_test,
            supplied_iv
        )

    print("=" * 60)
    print("PADDING ORACLE ATTACK")
    print("=" * 60)

    print("\nCiphertext:")
    print(ciphertext.hex())

    print("\nIV:")
    print(iv.hex())

    print("\nStarting attack...")

    recovered = attack(
        ciphertext,
        iv,
        oracle
    )

    print("\nRecovered plaintext:")
    print(recovered.decode())

    print("\nOracle queries:")
    print(oracle_object.query_count)

    print("\nOriginal plaintext:")
    print(original_message)

    print("\nRecovery successful:",
          recovered == original_message)


if __name__ == "__main__":
    main()