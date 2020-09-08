import util

def testAdd(number1, number2):
    result = hex((number1 + number2) & util.bitMask)[2:]

    return util.test(result, "add {0} {1}".format(
        util.toBigIntString(number1), util.toBigIntString(number2))
    )

def testSubtract(number1, number2):
    result = number1 - number2

    if result < 0:
        power = max((len(hex(-result)) // 8 + 1) * 32, 32)
        result = (1 << power) + result
        result = hex(result)[2:]
        # Workaround for bizarre "f" and "0" prefixing.
        while result[:8] == "f" * 8: result = result[8:]
        while result[0] == '0': result = result[1:]
    else:
        result = hex(result)[2:]

    return util.test(result, "subtract {0} {1}".format(
        util.toBigIntString(number1), util.toBigIntString(number2))
    )

def testMultiply(number1, number2):
    result = hex((number1 * number2) & util.bitMask)[2:]

    return util.test(result, "multiply {0} {1}".format(
        util.toBigIntString(number1), util.toBigIntString(number2))
    )

def testDivide(number1, number2):
    result = hex((number1 // number2) & util.bitMask)[2:]

    return util.test(result, "divide {0} {1}".format(
        util.toBigIntString(number1), util.toBigIntString(number2))
    )

def testMod(number1, number2):
    result = hex((number1 % number2) & util.bitMask)[2:]

    return util.test(result, "mod {0} {1}".format(
        util.toBigIntString(number1), util.toBigIntString(number2))
    )

def testModExp(number1, number2, number3):
    result = hex(pow(number1, number2, number3) & util.bitMask)[2:]

    return util.test(result, "modexp {0} {1} {2}".format(
        util.toBigIntString(number1), util.toBigIntString(number2), util.toBigIntString(number3))
    )

def testAdds(printing):
    for i in range(util.attempts):
        n1, n2 = util.randomInRange(), util.randomInRange()

        if not testAdd(n1, n2):
            if printing: print("Error in testAdd.")
            return False
    if printing: print("Add tests successful.")
    return True

def testSubtracts(printing):
    for i in range(util.attempts):
        n1, n2 = util.randomInRange(), util.randomInRange()

        if not testSubtract(n1, n2):
            if printing: print("Error in testSubtract.")
            return False
    if printing: print("Subtract tests successful.")
    return True

def testMultiplies(printing):
    for i in range(util.attempts):
        n1, n2 = util.randomInRange(),  util.randomInRange()

        if not testMultiply(n1, n2):
            if printing: print("Error in testMultiply.")
            return False
    if printing: print("Multiply tests successful.")
    return True

def testDivides(printing):
    for i in range(util.attempts):
        n1, n2, n3 = util.randomInRange(), util.randomInRange(), util.random.randint(0, 1 << (util.numBits // 2))

        if not testDivide(n1, n2):
            if printing: print("Error in testDivide.")
            return False
        if not testDivide(n1, n3):
            if printing: print("Error in testDivide.")
            return False
        if not testMod(n1, n2):
            if printing: print("Error in testMod.")
            return False
        if not testMod(n1, n3):
            if printing: print("Error in testMod.")
            return False
    if printing: print("Divide tests successful.")
    return True

def testModExps(printing):
    for i in range(util.attempts):
        n1, n2, n3 = util.random.randint(2, 10), util.random.randint(1, 290), util.random.randint(2, 1000000)
        if not testModExp(n1, n2, n3):
            if printing: print("Error in testModExp.")
            return False
    if printing: print("Mod exp tests succeeded.")
    return True

def testAll(printing = True):
    if not testAdds(printing): return False
    if not testSubtracts(printing): return False
    if not testMultiplies(printing): return False
    if not testDivides(printing): return False
    if not testModExps(printing): return False

    if printing: print("Arithmetic tests successful.")
    return True

if __name__ == "__main__":
    testAll()
