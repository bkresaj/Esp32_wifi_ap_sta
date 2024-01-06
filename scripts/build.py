#!/usr/bin/env python3

import os
import subprocess

PARAM = ""
TARGET = "esp32"
IDF_PATH = "/root/esp-idf"

if TARGET == "esp32":
    PARAM = f"-DCMAKE_TOOLCHAIN_FILE={IDF_PATH}/tools/cmake/toolchain-{TARGET}.cmake"

build_dir = "/workspace/main/build"

if not os.path.exists(build_dir):
    os.makedirs(build_dir)

os.chdir(build_dir)

subprocess.run(["cmake", PARAM, "-GNinja", ".."], check=True)
subprocess.run(["cmake", "--build", "."], check=True)
