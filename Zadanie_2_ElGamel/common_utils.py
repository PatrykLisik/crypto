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
        # base with result
        if (exponent & 1) == 1:
            result = (result * base) % modulo

        # exponent must be even now
        exponent = exponent >> 1  # exponent = exponent/2
        base = (base * base) % modulo

    return result
