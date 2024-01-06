#!/usr/bin/env python3

import os
import subprocess
import argparse

parser = argparse.ArgumentParser(description="Python translation of your script")
parser.add_argument("--port", required=True, help="Specify the port")

args = parser.parse_args()

subprocess.run(["python", "-m", "esp_idf_monitor", "-p", args.port])
