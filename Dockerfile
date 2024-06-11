FROM ubuntu:22.04

ENV TZ=Europe/Berlin
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update && \
    apt-get install -y \
    software-properties-common \
    wget \
    curl

# Install gcc 13
RUN add-apt-repository ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install -y g++-13 && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 100 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-13 100

# Install clang-18
RUN wget https://apt.llvm.org/llvm.sh && \
    chmod u+x llvm.sh 
RUN ./llvm.sh 18 && \
    update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-18 100

# Install python3.12
RUN add-apt-repository ppa:deadsnakes/ppa && \
    apt-get update && \
    apt-get install -y python3.12 python3.12-distutils && \
    update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.12 100 && \ 
    curl -sS https://bootstrap.pypa.io/get-pip.py | python3

# Install other tools
RUN apt-get install -y \
    # cmake
    cmake \
    # version control system
    git

# Install compiler explorer python package
RUN pip3.12 install --ignore-installed cexpl

# Create directory structure and set working directory
RUN mkdir -p /tsl/generator /tsl/lib /tutorial
WORKDIR /tutorial
ENV TUTORIAL_HOME=/tutorial
ENV TUTORIAL_SCRIPTS=/tutorial/scripts

RUN mkdir -p ${TUTORIAL_SCRIPTS}

# Download TSL Repository for interactive building
RUN git clone https://github.com/db-tu-dresden/TSL.git /tsl/generator

# Install TSL dependencies
RUN pip3.12 install --ignore-installed -r /tsl/generator/requirements.txt


# Download TSL release
RUN curl -L "https://github.com/db-tu-dresden/TSL/releases/latest/download/tsl.tar.gz" -o /tsl/lib/tsl.tar.gz



# Copy everything from host:./docker to container:/tutorial
COPY docker/* ${TUTORIAL_SCRIPTS}/
RUN chmod +x ${TUTORIAL_SCRIPTS}/*.sh

# Setup compiler explorer environment
RUN UNAME=$(uname -m) && \
    if [ "$UNAME" = "aarch64" ]; then \
        UNAME="ARM64"; \
    elif [ "$UNAME" = "x86_64" ]; then \
        UNAME="x86-64"; \
    fi && \
    GCC_VERSION=$(gcc-13 --version | head -n 1 | awk '{print $4}' | cut -d'.' -f1,2) && \
    CLANG_VERSION=$(clang++-18 --version | head -n 1 | awk '{print $4}' | cut -d'.' -f1,2) && \
    CCEXP_CLANG=$(cexpl --list-compilers c++ | grep -P "${UNAME} clang ${CLANG_VERSION}\.\d+$" | awk '{print $1}') && \
    CCEXP_GCC=$(cexpl --list-compilers c++ | grep -P "x86-64 gcc ${GCC_VERSION}" | awk '{print $1}') && \
    echo "UNAME=${UNAME}" >> ${TUTORIAL_SCRIPTS}/compiler_explorer.env && \
    echo "GCC_VERSION=${GCC_VERSION}" >> ${TUTORIAL_SCRIPTS}/compiler_explorer.env && \
    echo "CLANG_VERSION=${CLANG_VERSION}" >> ${TUTORIAL_SCRIPTS}/compiler_explorer.env && \
    echo "CCEXP_CLANG=${CCEXP_CLANG}" >> ${TUTORIAL_SCRIPTS}/compiler_explorer.env && \
    echo "CCEXP_GCC=${CCEXP_GCC}" >> ${TUTORIAL_SCRIPTS}/compiler_explorer.env && \
    echo "CCEXP_FLAGS=\"-O2 -std=c++20 -DCOMPILER_EXPLORER\"" >> ${TUTORIAL_SCRIPTS}/compiler_explorer.env && \
    echo "CCEXP_SIMD_DEFAULT_FLAGS=\"-march=native\"" >> ${TUTORIAL_SCRIPTS}/compiler_explorer.env && \
    echo "CCEXP_SSE_FLAGS=\"-msse -msse2 -mssse3 -msse4.1 -msse4.2\"" >> ${TUTORIAL_SCRIPTS}/compiler_explorer.env && \
    echo "CCEXP_AVX_FLAGS=\"-msse -msse2 -mssse3 -msse4.1 -msse4.2 -mavx -mavx2\"" >> ${TUTORIAL_SCRIPTS}/compiler_explorer.env && \
    echo "CCEXP_AVX512_FLAGS=\"-msse -msse2 -mssse3 -msse4.1 -msse4.2 -mavx -mavx2 -mavx512f -mavx512dq -mavx512bw -mavx512vl -mavx512cd -mavx512ifma -mavx512vbmi -mavx512vnni -mavx512vpopcntdq\"" >> ${TUTORIAL_SCRIPTS}/compiler_explorer.env


ENTRYPOINT ["/tutorial/scripts/entrypoint.sh"]
