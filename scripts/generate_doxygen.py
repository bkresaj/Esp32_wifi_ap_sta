#!/usr/bin/env python3

import subprocess
import os

current_directory = os.path.dirname(os.path.abspath(__file__))

parent_directory = os.path.dirname(current_directory)

os.chdir(parent_directory)

subprocess.run(["doxygen", "docs/Doxyfile"], check=True)
