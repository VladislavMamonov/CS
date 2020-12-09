#include "dgemm.hpp"


double wtime()
{
  struct timeval t;
  gettimeofday(&t, NULL);

  return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
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


int main(int argc, char const *argv[])
{
  if (argc != 3) {
    cout << "Launch format: ./[program_name] [matrix size] [number of threads]" << endl;
    return 1;
  }

  int matrix_size = abs(atoi(argv[1]));
  int threads_num = abs(atoi(argv[2]));

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

  srand(time(NULL));
  init_matrix(matrix_size, a, b, c);
  int Lnum = 1;

  double time = wtime();
  for (int i = 0; i < Lnum; i++) {
    dgemm_omp(matrix_size, a, b, c, threads_num);
    //dgemm(matrix_size, a, b, c);
  }
  time = wtime() - time;
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
