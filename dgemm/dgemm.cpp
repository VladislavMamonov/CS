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



void dgemm_opt_1(int size, double **a, double **b, double **c)
{
  double temp;
  double sum = 0.0;

  for (int i = 0; i < size; i++) {
    for (int k = 0; k < size; k++) {
      temp = a[i][k];
      sum = 0.0;
      for (int j = 0; j < size; j++)
        sum += temp * b[k][j];
      c[i][k] = sum;
    }
  }
}



int dgemm_opt_2(int size, int block_size, double **a, double **b, double **c)
{
  double sum;
  if (size % block_size != 0) return 1;

  double **block = new double* [size];
  for (int i = 0; i < size; i++)
    block[i] = new double[size];

  for (int ii = 0; ii < size; ii += block_size) {
    for (int jj = 0; jj < size; jj += block_size) {
      int i_start = ii;
      int i_end = ii + block_size;
      int j_start = jj;
      int j_end = jj + block_size;

      // Обходим блок
      for (int i = i_start; i < i_end; i++) {
        for (int j = j_start; j < j_end; j++) {
          sum = 0.0;
          for (int k = j_start; k < j_end; k++)
            sum += a[i][k] * b[k][j];
          c[i][j] = sum;
        }
      }
    }
  }

  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      block[i][j] = c[i][j];

  // Суммируем результаты из каждого блока в результирующую матрицу
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      for (int k = i; k + block_size < size; k += block_size)
        c[i][j] += block[k][j];
      for (int k = i; k - block_size >= 0; k -= block_size)
        c[i][j] += block[k][j];
    }
  }

  for (int i = 0; i < size; i++)
    delete [] block[i];
  delete [] block;

  return 0;
}
