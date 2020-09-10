import random

import util

def millerRabin(n, k):
    if n < 2: return 0
    if n == 2 or n == 3: return 1

    if n % 2 == 0: return 0

    r = 0
    s = n - 1
    while not s % 2:
        r += 1
        s //= 2

    for i in range(k):
        a = random.randint(2, n - 1)
        x = pow(a, s, n)
        if x == 1 or x == n - 1:
            continue
        for j in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return 0
    return 1

def testMiller(number):
    result = str(millerRabin(number, 20))

    return util.test(result, "prime {0}".format(
        util.toBigIntString(number))
    )

def testPrimes(printing):
    for i in range(util.attempts):
        n = random.randint(2, 1000)
        if not testMiller(n):
            if printing: print("Error in testMiller.")
            return False
    if printing: print("Miller-Rabin tests successful.")
    return True

def testAll(printing = True):
    if not testPrimes(printing): return False

    return True

if __name__ == "__main__":
    testAll()
