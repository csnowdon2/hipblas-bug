#include <hiprand/hiprand.h>
#include <hip/hip_runtime.h>
#include <hipblas/hipblas.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

void gpuAssert(hipError_t status)
{
  if (status != hipSuccess)
    std::cerr << "GPU Error: " << hipGetErrorString(status);
}

void save_matrix(double* A, size_t m, size_t n, std::string filename) {
  std::vector<double> host_A(m*n);
  gpuAssert(hipMemcpy(host_A.data(), A, m*n*sizeof(double), 
                      hipMemcpyDeviceToHost));

  std::ofstream file(filename, std::ios::out);

  for (size_t i=0; i<m; i++) {
    for (size_t j=0; j<n; j++) {
      file << host_A[j*m + i];
      if (j<n-1)
        file << ",";
    }
    file << std::endl;
  }
}

int main(int argc, char *argv[]) {
  int m,k,n;

  if (argc == 4) {
    m = std::stoi(argv[1]);
    k = std::stoi(argv[2]);
    n = std::stoi(argv[3]);
  } else {
    std::cout << "USAGE: ./bug m k n" << std::endl;
    exit(1);
  }

  hipblasHandle_t handle;
  hipblasCreate(&handle);

  hiprandGenerator_t rng;
  hiprandCreateGenerator(&rng, HIPRAND_RNG_PSEUDO_DEFAULT);

  double alpha = 1.0;
  double beta = 0.0;

  double *A, *B, *C;
  gpuAssert(hipMalloc(&A, m*k*sizeof(double)));
  gpuAssert(hipMalloc(&B, n*k*sizeof(double)));
  gpuAssert(hipMalloc(&C, m*n*sizeof(double)));


  std::vector<double> host_A(m*k);
  for (auto &x : host_A) x = 0.0;
  host_A[0] = 1.0;

  gpuAssert(hipMemcpy(A, host_A.data(), m*k*sizeof(double), hipMemcpyHostToDevice));
  
  hiprandGenerateUniformDouble(rng, B, n*k);

  hipblasDgemm(handle,
              HIPBLAS_OP_N,
              HIPBLAS_OP_T,
              m, n, k,
              &alpha,
              A, m,
              B, n,
              &beta,
              C, m);

  save_matrix(A, m, k, "A.csv");
  save_matrix(B, n, k, "B.csv");
  save_matrix(C, m, n, "C.csv");

  hipblasDestroy(handle);
  hiprandDestroyGenerator(rng);
  gpuAssert(hipFree(A));
  gpuAssert(hipFree(B));
  gpuAssert(hipFree(C));

  return 0;
}
