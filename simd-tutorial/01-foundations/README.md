# Foundations

## Aggregation

```bash
git clone git@github.com:JPietrzykTUD/simd-tutorial.git
cd simd-tutorial
```
```bash
docker run -v $(pwd)/sigmod-tutorial:/tutorial/code -it jpietrzyktud/sigmod-simd-tutorial:latest /tutorial/scripts/compile.sh /tutorial/code/foundations/include/aggregation/aggregate_scalar.hpp gcc
```
```bash
docker run -v $(pwd):/tutorial/code -it jpietrzyktud/sigmod-simd-tutorial:latest /tutorial/scripts/inspect.sh /tutorial/code/foundations/include/aggregation/aggregate_scalar.hpp gcc
```