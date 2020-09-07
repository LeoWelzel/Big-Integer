import arithmetic, assignment, bitwise

def testAll(printing = True):
    if not arithmetic.testAll():
        if printing: print("Error in arithmetic tests."); return
    if not assignment.testAll():
        if printing: print("Error in assignment tests."); return
    if not bitwise.testAll():
        if printing: print("Error in bitwise tests."); return
    if printing: print("All tests successful.")

if __name__ == "__main__":
    testAll()