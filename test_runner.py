#!/bin/env python3

import sys
import subprocess
import random

modes = {"1": "-s", "2": "-si", "3": "-sh"}

command, mode, count, floor, ceil = sys.argv[-1].split()
mode = modes[mode]
count = int(count)
floor = float(floor)
ceil = float(ceil)

array = [random.uniform(floor, ceil) for _ in range(count)]
output = f" ".join(f"{n:.6f}".replace(".", ",") for n in sorted(array))

result = subprocess.run(
    [command, mode, *(f"{n:.6f}".replace(".", ",") for n in array)], capture_output=True
)
if bytes(output, encoding="UTF-8") not in result.stdout.splitlines()[-1]:
    print(
        "> Expected output:",
        output,
        "> Test has not passed. Program output:",
        result.stdout.decode(),
        "> Error log:",
        result.stderr.decode(),
        sep="\n",
    )
    sys.exit(1)
sys.exit(0)
