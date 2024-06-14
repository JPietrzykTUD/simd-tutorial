# SIMDified Data Processing - <br>Foundations, Abstraction, and Advanced Techniques

Supporting material for the tutorial on SIMDified Data Processing [1] @ 2024 ACM SIGMOD/PODS @Santiago, Chile.


The provided material is tested under Linux and Windows with WSL2 on an x86 maschine. 


## Prerequisites

#### 0. Install git.
#### 1. Install [Docker](https://docs.docker.com/engine/install/) depending on your maschine. 
#### 2. Clone this repository either using the _SSH_ or _HTTPS_ version:
```bash
git clone git@github.com:db-tu-dresden/SIGMOD-SIMD-tutorial.git
```
```bash
git clone https://github.com/db-tu-dresden/SIGMOD-SIMD-tutorial.git
```
#### 3. Navigate into the checked-out repository
```bash
cd SIGMOD-SIMD-tutorial
```
#### 4. Pull and tag the associated docker image, containing all the necessary tools and libraries for this tutorial. This may take a couple of minutes.
```bash
docker pull jpietrzyktud/sigmod-simd-tutorial:latest
docker tag jpietrzyktud/sigmod-simd-tutorial:latest sigmod-simd
```
#### 5. Verify, that you are able to run the container and build the code
```bash
docker run -v $(pwd):/tutorial/code -it sigmod-simd scripts/run.sh /tutorial/code help
```
This should result in the following ouptut:
```bash
[user@pc ~/SIGMOD-SIMD-tutorial] docker run -v $(pwd):/tutorial/code -it sigmod-simd scripts/run.sh /tutorial/code help
### Got variable scripts/run.sh ###
### Got variable /tutorial/code ###
### Got variable help ###
### Running non-interactive mode ###
-- The C compiler identification is GNU 13.1.0
-- The CXX compiler identification is GNU 13.1.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/gcc-13 - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/g++-13 - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /tutorial/code/build
The following are some of the valid targets for this Makefile:
... all (the default if no target is provided)
... clean
... depend
... edit_cache
... rebuild_cache
... abstraction_x86_aggregate_sum
... abstraction_x86_filter_sum
... abstraction_x86_group_sum
... abstraction_x86_nsm_dsm
... advanced_concepts_x86_nsm_dsm
... foundations_x86_aggregate_sum
... foundations_x86_filter_sum
... foundations_x86_group_sum
... foundations_x86_nsm_dsm
```


## Commands from the slides

| Part                   |   Slide   | Command       |
|------------------------|:---------:|---------------|
| 01 - Foundations       |     27    |```docker run -v $(pwd):/tutorial/code -it sigmod-simd scripts/asm_inspect.sh code/simd-tutorial/01-foundations/include/aggregation/sum_scalar.hpp gcc```|
| 01 - Foundations       |     28    |```docker run -v $(pwd):/tutorial/code -it sigmod-simd scripts/asm_inspect.sh code/simd-tutorial/01-foundations/include/aggregation/sum_nested_loop.hpp gcc```|
| 02 - Abstraction       |     39    |```docker run -v $(pwd):/tutorial/code -it sigmod-simd scripts/run.sh abstraction_x86_aggregate_sum```|
| 03 - Advanced Concepts |     73    |```docker run -v $(pwd):/tutorial/code -it sigmod-simd scripts/run.sh advanced_concepts_x86_nsm_dsm```|



[1] [Link to the paper](https://doi.org/10.1145/3626246.3654694)