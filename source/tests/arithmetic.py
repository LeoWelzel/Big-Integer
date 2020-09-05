if __name__ == "__main__": import util
else: from . import util

def testAdd(number1, number2):
    result = hex((number1 + number2) & util.bitMask)[2:]

    return util.test(result, "add {0} {1}".format(
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

def testAll(printing = True):
    if not testAdds(printing): return False

    if printing: print("Arithemetic tests successful.")
    return True

if __name__ == "__main__":
    testAll()
