if __name__ == "__main__": import util
else: from . import util

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

def testOr(number1, number2):
    result = hex((number1 | number2) & util.bitMask)[2:]
    
    return util.test(result, "or {0} {1}".format(
        util.toBigIntString(number1), util.toBigIntString(number2))
    )

def testAnd(number1, number2):
    result = hex((number1 & number2) & util.bitMask)[2:]
    
    return util.test(result, "and {0} {1}".format(
        util.toBigIntString(number1), util.toBigIntString(number2))
    )

def testXor(number1, number2):
    result = hex((number1 ^ number2) & util.bitMask)[2:]
    
    return util.test(result, "xor {0} {1}".format(
        util.toBigIntString(number1), util.toBigIntString(number2))
    )

def testShifts(printing):
    for i in range(util.attempts):
        number = util.randomInRange()
        shift = util.randomInRange()

        if not testLShift(number, shift):
            if printing: print("Error in lshift.")
            return False
        if not testRShift(number, shift):
            if printing: print("Error in rshift.")
            return False
    if printing: print("Shift tests completed.")
    return True

def testBinaryOps(printing):
    for i in range(util.attempts):
        num1, num2 = util.randomInRange(), util.randomInRange()
        if not testOr(num1, num2):
            if printing: print("Error in or.")
            return False
        if not testAnd(num1, num2):
            if printing: print("Error in and.")
            return False
        if not testXor(num1, num2):
            if printing: print("Error in xor.")
            return False
    if printing: print("Binary operation tests completed.")
    return True

def testAll(printing = True):
    testShifts(printing)
    testBinaryOps(printing)
    
    if printing: print("Bitwise tests successful.")
    return True

if __name__ == "__main__":
    testAll()
