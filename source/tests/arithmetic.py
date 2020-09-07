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


def testAll(printing = True):
    if not testAdds(printing): return False
    if not testSubtracts(printing): return False
    if not testMultiplies(printing): return False

    if printing: print("Arithmetic tests successful.")
    return True

if __name__ == "__main__":
    testAll()
