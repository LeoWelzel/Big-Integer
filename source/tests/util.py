import subprocess, os, random

binDir = "../../bin"
numBits = 1024
bitMask = (1 << numBits) - 1
maxInt = 2 ** 32

# The default number of times a particular test is performed.
attempts = 100

# Do not prefix commands with BigInt.exe.
def getCmdOutput(command):
    result = subprocess.run(
        "cd {0} & BigInt.exe {1}".format(binDir, command), capture_output=True, shell=True
    )
    return result.stdout.decode("utf8")

def test(pythonResult, command):
    output = getCmdOutput(command)
    result = str(pythonResult) == output
    if not result: print(command, "Expected: {{{0}}} Actual: {{{1}}}".format(pythonResult, output))
    return result

def toBigIntString(integer):
    sInteger = hex(integer)[2:]
    if len(sInteger) % 8 == 0: return sInteger
    else: return sInteger.zfill(8 * (len(sInteger) // 8 + 1))

def randomInRange():
    return random.randint(0, bitMask)
