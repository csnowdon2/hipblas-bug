# hipblas-bug
Code and scripts to reproduce a bug in hipBLAS. 

The bug has been detected when performing a `hipblasDgemm` of the form $C = AB^T$ with `(m,k,n) = (375,375,4309)`. The code in this repository demonstrates the bug by performing such a `hipblasDgemm` call while $A$ is initialized to be zero except for a $1.0$ in the top-left corner. We would thus expect the output $C$ to be zero except for the first row, which should be equal to the first column of $B$. However, we find that $C$ has non-zero entries outside of the first row. 

Sample matrices $A,B,C$ generated by this program with ROCm 5.7.3 on the Setonix supercomputer at the Pawsey Supercomputing Centre in Perth, Australia, can be found in the `outputs` folder.

The bug is also present when $A$ is initialized with random values, but it is particularly apparent in this case.

## `bug.cpp`
Usage: `./bug m k n`

Performs a `hipblasDgemm` of the form $C = AB^T$, where $A,B,C$ have the following properties:
- $A$ is an $m \times k$ matrix which is zero except for a 1.0 in the top-left corner. 
- $B$ is a $n \times k$ matrix with entries randomly generated by `hiprandGenerateUniformDouble`
- $C$ is an $m \times n$ matrix, initialized to zero.
The matrices are saved in `A.csv`, `B.csv` and `C.csv`. 

## `check_matrices.py`
Usage: `python3 check_matrices.py`

Loads the matrices from `A.csv`, `B.csv` and `C.csv`. Uses `pandas` to calculate $C - AB^T$, and prints the norm. If the original calculation of $C$ were correct, the norm should be close to zero. A large norm indicates an error in the original product.

## `expose_bug.sh`
Usage: `./expose_bug.sh`

Uses the above two scripts to demonstrate an error in `hipblasDgemm` for `(m,k,n) = (375,375,4309)`. 
