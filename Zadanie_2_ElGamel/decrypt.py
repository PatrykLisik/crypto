from common_utils import random_n_digits_prime, power_mod, mod_inverse
import random





if __name__ == '__main__':
    # https://pl.wikipedia.org/wiki/ElGamal
    prime = random_n_digits_prime(30)
    alpha = random.randint(2, prime - 1)
    secret_number = random.randint(1, prime)
    beta = power_mod(alpha, secret_number, prime)
    print(prime, file=open("prime.txt", "w"))
    print(alpha, file=open("alpha.txt", "w"))
    print(beta, file=open("beta.txt", "w"))
    print("Public key has been published")

    file_name = input("Enter the name of encrypted file: ")
    foreign_key_file = input("Enter the name of file with encryption public key: ")

    foreign_key = int_from_file(foreign_key_file)
    k_m = power_mod(foreign_key, secret_number, prime)
    inverse = mod_inverse(k_m, prime)

    for line in open(file_name, "r"):
        print(chr(int(line) * inverse % prime), end="")
    print()
