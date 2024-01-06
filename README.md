# Project Description

This project enables the ESP32 to function as a WiFi access point initially, allowing users to connect to it. Once connected, users can enter their network SSID and password. The ESP32 will then transition to station mode, connecting to the specified network and retrieving time and date information from an NTP (Network Time Protocol) server.

## Overview

The project is built on the ESP32 platform using the ESP-IDF (Espressif IoT Development Framework). It aims to provide functionalities involving WiFi access point and station modes, as well as support for SPIFFS (SPI Flash File System) and an NTP (Network Time Protocol) server.

## Technology Stack

- **Languages:** C, C++, Python
- **Build System:** CMake
- **Containerization:** Docker

## Implementation Details

The project utilizes a combination of C, C++, and Python scripting to implement various features. 

- C and C++ are primarily used for firmware development on the ESP32 platform, leveraging the capabilities of the ESP-IDF.
- Python scripts handle code building, flashing, logging, unit testing using gtest/gmock frameworks, coverage analysis using gcov, valgrind support for memory-related issues, documentation generation using doxygen, and static code analysis using cppcheck.
- Build system for this project relies on CMake, which simplifies the process of building the project and managing dependencies. 
- Containerization is achieved using Docker for lightweight deployment.

## GitHub Actions Workflow

GitHub Actions automate the build, unit tests, and static analysis process. The following jobs are defined:

- **Build:** Builds the source code.
- **Unit Tests:** Runs the unit tests for the project.
- **Static Analysis with Cppcheck:** Identifies potential errors and vulnerabilities in the code.

The GitHub Actions workflow triggers whenever a commit is pushed to any branch of the repository, ensuring the project is continuously built, tested, and analyzed in a pipeline.

## Conclusion

This project combines multiple technologies, including ESP-IDF, C, C++, Python, CMake, Docker, gcov, valgrind, cppcheck, Doxygen, gtest, and gmock. It delivers a robust and efficient solution for ESP32-based applications with WiFi capabilities, SPIFFS support, and an NTP server.


## How to use it:

**Warning:** When connecting to the ESP32 server using HTTP (HyperText Transfer Protocol), please be aware that the communication occurs without encryption or security measures. This means that any data transmitted between the client and the ESP32 can potentially be intercepted or modified by malicious actors.
TODO: Use HTTPS for secure communication


### Clone repository:
```bash
git clone https://github.com/bkresaj/Esp32_wifi_ap_sta.git
```

### Build docker image:
```bash
(cd .devcontainer && docker build . -t=esp32_wifi_sta_ap:latest)
```

### Find your docker image:
```bash
docker images
```

### Run container from it:
```bash
docker run -it --rm --privileged --device=<device_name_of_esp32_when_connected> -v ${PWD}:/workspace --entrypoint bash <image_id>
```

### After running docker container:

### Build source code:
```bash
./scripts/build.py
```

### Flash code to esp32:
```bash
./scripts/flash.py --port <device_name_of_esp32_when_connected>
```

### Monitor logs from esp32:
```bash
./scripts/monitor.py --port <device_name_of_esp32_when_connected>
```

### Clean build files:
```bash
./scripts/clean.py
```

### Generate Doxygen documentation:
```bash
./scripts/generate_doxygen.py
```

### Run static analysis with cpp check:
```bash
./scripts/run_cpp_checker.py
```

#### NOTE: When rerunning unit tests with different arguments, consider cleaning the tests/build folder and then rerun them again.
### Run unit tests:
```bash
./scripts/run_unit_tests.py
```

### Run unit tests with coverage informations(either on terminal or HTML files):
```bash
./scripts/run_unit_tests.py COVERAGE/HTML_COVERAGE
```

### Run unit tests with valgrind:
```bash
./scripts/run_unit_tests.py VALGRIND
```

### Run unit tests with coverage informations and valgrind:
```bash
./scripts/run_unit_tests.py COVERAGE/HTML_COVERAGE VALGRIND
```

