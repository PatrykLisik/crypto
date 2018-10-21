#!/bin/bash/python3

from random import randrange
import Sha
import Primes
import sys
import tempfile


def p_gen(q):
    while True:
        n = randrange(pow(2, 863), pow(2, 864)-1)
        p = (q * n) + 1
        if Primes.is_prime(p, 512):
            return p


def g_gen(p, q):
    h = 2
    x = (p-1)//q
    while True:
        g = pow(h, x, p)
        if g != 1:
            return g
        h += 1


def s_gen(k, sha, x, r, q):
    mul = sha + (x * r)
    s = (pow(k, q-2, q) * mul) % q
    return s


# Choose 160 bit prime q.
# With second parameter being 512, there is only 7.458341e-157 % chance that something goes wrong
q = Primes.generate_prime_number(160, 512)
print("Parameters:\n")
print("q: ", q)
# Choose 1024 bit prime p such that p-1 is a multiple of q
p = p_gen(q)
print("p: ", p)
# Choose g, a number whose multiplicative order modulo p is q
g = g_gen(p, q)
print("g: ", g)
# Choose user secret key x: 0 < x < q
x = randrange(1, q-1)
print("\nPer-user keys:\n")
print("x: ", x)
# Calculate the public key y = g**x mod p
y = pow(g, x, p)
print("y: ", y)
# Get sha of file or input string
if sys.argv[1] == '-i':
    try:
        filename = sys.argv[2]
        with open(filename, 'rb') as f:
            content = f.read()
    except:
        print('ERROR: Input file "{0}" cannot be read.'.format(filename))
    else:
        h = Sha.SHA1()
        h.update(content)
elif sys.argv[1] == '-m':
    content = ""
    for word in sys.argv[3:]:
        content += word
    h = Sha.SHA1()
    h.update(content.encode())
else:
    raise Exception('Bad arguments!')


sha = int(str(h), 16)
print("\nFile hash:\n")
print("sha-1: ", sha)
# Signing (r, s)
# Random, per-message value k, 1 < k < q
r = 0
s = 0
while r == 0 or s == 0:
    k = randrange(2, q-1)
# Generate r = ((g**k)%p)%q
    r = pow(g, k, p) % q
# Generate s = k**-1(sha+xr)%q
    s = s_gen(k, sha, x, r, q)
print("\nSigning:\n")
print("k: ", k)
print("r: ", r)
print("s: ", s)
# Verification
w = pow(s, q-2, q)
print("\nVerification:\n\nw: ", w)
u1 = (sha * w)
print("u1: ", u1)
u2 = (r * w)
print("u2: ", u2)
v = ((pow(g, u1, p) * pow(y, u2, p)) % p) % q
print("v: ", v)
if v == r:
    print("\nSIGNATURE IS VALID")

