#!/usr/bin/env python3

import subprocess
import os
import sys

CMAKE_ARGUMENT_1 = ""
CMAKE_ARGUMENT_2 = ""
RUN_COVERAGE_FLAG = False
HTML_COVERAGE_FLAG = False


def build_project():
    os.chdir("/workspace/tests")
    if os.path.exists("/workspace/tests/build") and os.path.isdir(
        "/workspace/tests/build"
    ):
        try:
            os.chdir("/workspace/tests/build")
            subprocess.check_call(["cmake", CMAKE_ARGUMENT_1, CMAKE_ARGUMENT_2, ".."])
            subprocess.check_call(["cmake", "--build", "."])
        except subprocess.CalledProcessError:
            sys.exit(1)

    else:
        try:
            subprocess.check_call(["mkdir", "build"])
            os.chdir("/workspace/tests/build")
            subprocess.check_call(["cmake", CMAKE_ARGUMENT_1, CMAKE_ARGUMENT_2, ".."])
            subprocess.check_call(["cmake", "--build", "."])
        except subprocess.CalledProcessError:
            sys.exit(1)


def run_tests():
    os.chdir("/workspace/tests/build")
    subprocess.call(["ctest"])


def run_coverage():
    print("Coverage results: ")
    os.chdir("/workspace/tests/build")
    subprocess.call(["make", "coverage"])
    if HTML_COVERAGE_FLAG:
        print("Coverage html are inside /workspace/tests/build/coverage folder")


def get_cmake_arguments():
    options = sys.argv

    global CMAKE_ARGUMENT_1
    global CMAKE_ARGUMENT_2
    global RUN_COVERAGE_FLAG
    global HTML_COVERAGE_FLAG

    if "COVERAGE" in options:
        CMAKE_ARGUMENT_1 = "-DCOVERAGE=ON"
        RUN_COVERAGE_FLAG = True
    elif "HTML_COVERAGE" in options:
        CMAKE_ARGUMENT_1 = "-DHTML_COVERAGE=ON"
        RUN_COVERAGE_FLAG = True
        HTML_COVERAGE_FLAG = True

    if "VALGRIND" in options:
        CMAKE_ARGUMENT_2 = "-DVALGRIND=ON"


if __name__ == "__main__":
    if len(sys.argv) > 1 and len(sys.argv) < 4:
        get_cmake_arguments()

    build_project()
    run_tests()
    print(
        "Output from tests are inside /workspace/tests/build/Testing/Temporary folder."
    )
    if RUN_COVERAGE_FLAG:
        run_coverage()
