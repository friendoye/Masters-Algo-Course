import math

def is_prime(x):
    d = math.floor(math.sqrt(x))
    while d > 1:
        if x % d == 0:
            return False
        d = d - 1
    return True

def get_primes(n):
    return [x for x in xrange(2, n + 1) if is_prime(x)]

if __name__ == "__main__":
    print get_primes(1)