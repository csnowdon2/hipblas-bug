# hipblas-bug
Code and scripts to reproduce a bug in hipBLAS

## `bug.cpp`
Usage: `./bug m k n`

Performs a `hipblasDgemm` of the form $C = AB^T$, with $A$ an $m \times k$ matrix, $B$ a $n \times k$ matrix and $C$ a $m \times n$ matrix. Saves the three matrices as `A.csv`, `B.csv`, `C.csv`.

## `check_matrices.py`
Usage: `python3 check_matrices.py`

Loads the matrices from `A.csv`, `B.csv` and `C.csv`. Uses `pandas` to calculate $C - AB^T$, and prints the norm. If the original calculation of $C$ were correct, the norm should be close to zero. A large norm indicates an error in the original product.

## `expose_bug.sh`
Usage: `./expose_bug.sh`

Uses the above two scripts to demonstrate an error in `hipblasDgemm` for `(m,k,n) = (375,375,4309)`. This has thus far been verified with ROCm 5.7.3 on the Setonix supercomputer at the Pawsey Supercomputing Centre in Perth, Australia.
