#include "dgemm.hpp"


void init_matrix(int size, double **a, double **b, double **c)
{
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      a[i][j] = 1.0;
      b[i][j] = 2.0;
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



void dgemm_opt_2(double *a, double *b, double *c, int n)
{

}
