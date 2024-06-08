# simd-tutorial

docker run -v $(pwd):/tutorial/code -it jpietrzyktud/sigmod-simd-tutorial:latest /tutorial/scripts/inspect.sh /tutorial/code/foundations/include/aggregation/aggregate_nested_loop.hpp gcc

docker run -v $(pwd):/tutorial/code -it jpietrzyktud/sigmod-simd-tutorial:latest cmake -S /tutorial/code -B /tutorial/code/build 
docker run -v $(pwd):/tutorial/code -it jpietrzyktud/sigmod-simd-tutorial:latest cmake --build /tutorial/code/build