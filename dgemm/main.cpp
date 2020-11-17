#include "dgemm.hpp"


int main(int argc, char const *argv[])
{
  if (argc != 2) {
    cout << "Launch format: ./[program_name] [matrix_size]" << endl;
    return 1;
  }

  int matrix_size = abs(atoi(argv[1]));

  //init matrix
  double **a = new double* [matrix_size];
  double **b = new double* [matrix_size];
  double **c = new double* [matrix_size];

  for (int i = 0; i < matrix_size; i++) {
    a[i] = new double[matrix_size];
    b[i] = new double[matrix_size];
    c[i] = new double[matrix_size];
  }

  if (a == NULL || b == NULL || c == NULL) {
    cout << "Memory allocation failed" << endl;
    return 1;
  }

  init_matrix(matrix_size, a, b, c);
  int Lnum = 3;

  double time = clock();
  for (int i = 0; i < Lnum; i++) {
    dgemm_opt_1(matrix_size, a, b, c);
  }
  time = (clock() - time) / CLOCKS_PER_SEC;
  time /= Lnum;
  //print_matrix(c, matrix_size);
  cout << "time: " << time << " sec." << endl;

  //delete matrix
  for (int i = 0; i < matrix_size; i++) {
    delete [] a[i];
    delete [] b[i];
    delete [] c[i];
  }
  delete [] a;
  delete [] b;
  delete [] c;
  return 0;
}
