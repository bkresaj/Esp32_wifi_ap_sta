#!/usr/bin/env python3

import os
import subprocess

current_directory = os.path.dirname(os.path.abspath(__file__))
parent_directory = os.path.dirname(current_directory)
os.chdir(parent_directory)
subprocess.run(["./scripts/build.py"])

output_directory = "output"

if not os.path.exists(output_directory):
    os.makedirs(output_directory, exist_ok=True)

subprocess.run(
    [
        "cppcheck",
        "--enable=all",
        "--inline-suppr",
        "--output-file=output/report.log",
        "--project=main/build/compile_commands.json",
    ]
)

with open("output/report.log", "r") as f:
    content = f.read()

# Remove non-printable characters
content_clean = "".join(
    ch for ch in content if ch.isprintable() or ch == "\t" or ch == "\n"
)

# Remove the [number] patterns
for number in range(101):
    content_clean = content_clean.replace(f"[{number}m", "")

# Split on the sequence ' ^' and join with newlines
sentences = content_clean.split(" ^")
content_clean = "\n".join(sentences)

# Write back the cleaned content
with open("output/report.log", "w") as f:
    f.write(content_clean)

print(
    "Output from cppcheck is inside "
    + parent_directory
    + "/"
    + output_directory
    + " folder."
)
