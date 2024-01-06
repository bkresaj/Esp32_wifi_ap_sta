#!/usr/bin/env python3

import os
import subprocess
import argparse

parser = argparse.ArgumentParser(description="Python translation of your script")
parser.add_argument("--port", required=True, help="Specify the port")

args = parser.parse_args()

if os.path.exists("/workspace/main/build"):
    os.chdir("/workspace/main/build")
else:
    current_directory = os.path.dirname(os.path.abspath(__file__))
    parent_directory = os.path.dirname(current_directory)
    os.chdir(parent_directory)
    subprocess.run(["./scripts/build.py"])

os.chdir("/workspace/main/build")
IDF_PATH = os.environ.get("IDF_PATH")
subprocess.run(
    [
        "python",
        os.path.join(IDF_PATH, "components", "esptool_py", "esptool", "esptool.py"),
        "-p",
        args.port,
        "write_flash",
        "@flash_project_args",
    ]
)
