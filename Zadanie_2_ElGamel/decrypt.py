from common_utils import random_n_digits_prime, power_mod
import random

if __name__ == '__main__':
    # https://pl.wikipedia.org/wiki/ElGamal
    p = random_n_digits_prime(10)
    alpha = random.randint(2, p - 1)
    k = random.randint(1, p)
    beta = power_mod(alpha, k, p)
    print(p, file=open("prime.txt", "w"))
    print(alpha, file=open("alpha.txt", "w"))
    print(beta, file=open("beta.txt", "w"))
