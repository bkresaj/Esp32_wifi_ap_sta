FROM python

RUN apt-get update
RUN apt-get -y install gdb cmake valgrind gcovr cppcheck doxygen graphviz python3 python3-pip openssl

RUN apt-get install -y flex bison gperf cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0

WORKDIR /root/gtest
RUN git clone --depth=1 -b v1.14.0 --recursive https://github.com/google/googletest.git

ENV IDF_PATH "/root/esp-idf"

WORKDIR /root
RUN git clone --depth=1 -b v5.0.2 --recursive https://github.com/espressif/esp-idf.git

RUN ${IDF_PATH}/install.sh
RUN echo ". ${IDF_PATH}/export.sh ; pip3 install esp-idf-monitor" >> /root/.bashrc

WORKDIR /workspace
