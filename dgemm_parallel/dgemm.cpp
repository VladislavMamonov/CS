#include "dgemm.hpp"


void init_matrix(int size, double **a, double **b, double **c)
{
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      a[i][j] = (double)rand();
      b[i][j] = (double)rand();
      c[i][j] = 0.0;
    }
  }
}



void print_matrix(double **a, int n)
{
  int i, j;

  cout << "Matrix: " << endl;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      cout << a[i][j] << "\t";
    }
    cout << endl;
  }
}



void dgemm(int size, double **a, double **b, double **c)
{
  double sum;

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      sum = 0.0;
      for (int k = 0; k < size; k++)
        sum += a[i][k] * b[k][j];
      c[i][j] = sum;
    }
  }
}


void dgemm_omp(int size, double **a, double **b, double **c, int threads_num)
{
  double sum;

  #pragma omp parallel num_threads(threads_num)
  {
    #pragma omp for reduction(+:sum)
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        sum = 0.0;
        for (int k = 0; k < size; k++)
          sum += a[i][k] * b[k][j];
        c[i][j] = sum;
      }
    }
  }
}
