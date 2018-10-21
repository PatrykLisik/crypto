import random
from common_utils import power_mod, int_from_file


def encrypt_chr(char: str, key: int, mod: int) -> int:
    """
    Helper function that encrypts single number
    :param char: character to encrypt
    :param key:
    :param mod:
    :return:
    """
    return ord(char) * key % mod


if __name__ == '__main__':
    # based on https://www.youtube.com/watch?v=tKNY1zhK3sQ&t=1524s
    beta = int_from_file("beta.txt")
    prime = int_from_file("prime.txt")
    alpha = int_from_file("alpha.txt")
    secret_number = random.randint(2, prime - 1)

    # ephemeral key
    k_e = power_mod(alpha, secret_number, prime)
    # masking/session key
    k_m = power_mod(beta, secret_number, prime)
    print("Masking key generated")
    in_file_name = input("Enter the name of file to perform encryption on: ")
    out_file_name = input("Enter the name of file to save encrypted data: ")

    with open(out_file_name, "w") as file:
        for line in open(in_file_name, "r"):
            for character in line:
                file.write("{}\n".format(encrypt_chr(character, k_m, prime)))

    print("Encrypted file save as encrypted.txt")

    print(k_e, file=open("encrypt_key.txt", "w"))
    print("Public key saved to encrypt_key.txt")
