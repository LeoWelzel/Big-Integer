import arithmetic, assignment, bitwise, millerrabin

def testAll(printing = True):
    if not arithmetic.testAll(printing):
        if printing: print("Error in arithmetic tests."); return
    if not assignment.testAll(printing):
        if printing: print("Error in assignment tests."); return
    if not bitwise.testAll(printing):
        if printing: print("Error in bitwise tests."); return
    if not millerrabin.testAll(printing):
        if printing: print("Error in miller rabin tests."); return
    if printing: print("All tests successful.")

if __name__ == "__main__":
    testAll()