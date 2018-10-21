import random


def power_mod(base: int, exponent: int, modulo: int) -> int:
    """
    Function that computes base^exponent%modulo in an efficient way
    :param base: a number to multiply
    :param exponent: a number of multiplications
    :param modulo: a number to find the remainder from
    :return: the reminder of the exponent times multiplication of the base after division by the modulo
    """
    result = 1
    base = base % modulo

    while exponent > 0:

        # If exponent is odd, multiply
        if (exponent & 1) == 1:
            result = (result * base) % modulo

        # exponent is even
        exponent = exponent >> 1  # exponent = exponent/2
        base = (base * base) % modulo

    return result


def miller_rabin(n: int, k: int = 100) -> bool:
    """
    Miller_rabin primarily test 
    :param n: number to perform test on
    :param k: number of tests to perform
    :return: aggregated result of secret_number-number of tests
    """
    if n == 2:
        return True
    if n % 2 == 0:
        return False
    if n < 2:
        return False

    # n - 1 =2^exponent2*multiplier
    exponent2, multiplier = _factor_two_power_r_mul_s(n)
    for _ in range(k):
        if not _miller_rabin_test(n, exponent2, multiplier):
            return False
    return True


def _factor_two_power_r_mul_s(number: int) -> tuple:
    """
    Helper function that factors two numbers an exponent2 and a multiplier, such that number - 1 =2^exponent2*multiplier
    :param number:
    :return: tuple of two beforementioned integers the exponent2 and the multiplier
    """
    exponent2, multiplier = 0, number - 1
    while multiplier % 2 == 0:
        exponent2 += 1
        multiplier //= 2
    return exponent2, multiplier


def _miller_rabin_test(number: int, exponent2: int, multiplier: int) -> bool:
    """
    Helper function that performs single Miller-Rabin test
    :param number: odd number to perform test on
    :param exponent2: integer such that number - 1 =2^exponent2*multiplier
    :param multiplier: integer such that number - 1 =2^exponent2*multiplier
    :return: False if number is composite
    """

    # This code is brought you by https://www.youtube.com/watch?v=qdylJqXCDGs
    random_base = random.randrange(2, number - 1)
    x = power_mod(random_base, multiplier, number)
    if x == 1 or x == number - 1:
        return True

    for _ in range(exponent2 - 1):
        x = power_mod(x, 2, number)
        # x mod number == -1
        if x == number - 1:
            return True
        if x == 1:
            return False
    return False


def random_n_digits(n: int) -> int:
    """
    :param n: number of digits
    :return: random n digit number
    """
    range_start = 10 ** (n - 1)
    range_end = (10 ** n) - 1
    return random.randint(range_start, range_end)


def random_n_digits_prime(n: int) -> int:
    """
    :param n: number of digits
    :return: random n digit prime
    """
    while True:
        random_int = random_n_digits(n)
        if miller_rabin(random_int, 300):
            return random_int


def gcd(a: int, b: int) -> int:
    """
    :param a: number
    :param b: number
    :return: the greatest common divisor
    """
    if a == 0:
        return b
    return gcd(b % a, a)


def mod_inverse(number: int, m: int) -> int:
    """
    Modular multiplicative inverse of number mod m
    :param number: integer to find inverse mod m of
    :param m: prime number
    :return: a^-1 mod m
    """
    if not gcd(number, m) == 1:
        raise ValueError
    return power_mod(number, m - 2, m)


def int_from_file(file_name: str) -> int:
    with open(file_name, 'r') as file:
        return int(file.read())
