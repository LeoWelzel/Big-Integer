if __name__ == "__main__": import util
else: from . import util

import random

def testAssign(number):
    result = hex(number)[2:]
    nString = util.toBigIntString(number)

    return util.test(result, "assign {0}".format(nString))

def testAll(printing = True):
    for i in range(util.attempts):
        n = util.randomInRange()
        if not testAssign(n):
            if printing: print("Error in assignment.")
            return False
    if printing: print("Assignment tests completed.")
    return True

if __name__ == "__main__":
    testAll()
