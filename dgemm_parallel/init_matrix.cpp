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
