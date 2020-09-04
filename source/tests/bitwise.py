if __name__ == "__main__": import util
else: from . import util

import random

def testLShift(number, shiftBy):
    assert(shiftBy >= 0)
    result = hex((number << shiftBy) & util.bitMask)[2:]
    nString = util.toBigIntString(number)

    return util.test(result, "lshift {0} {1}".format(nString, shiftBy))

def testRShift(number, shiftBy):
    assert(shiftBy >= 0)
    result = hex(number >> shiftBy)[2:]
    nString = util.toBigIntString(number)

    return util.test(result, "rshift {0} {1}".format(nString, shiftBy))

def testAll(printing = True):
    for i in range(util.attempts):
        number = random.randint(0, util.bitMask)
        shift = random.randint(0, util.numBits)

        if not testLShift(number, shift):
            return False
        if not testRShift(number, shift):
            return False
    if printing: print("Bitwise tests successful.")

if __name__ == "__main__":
    testAll()
