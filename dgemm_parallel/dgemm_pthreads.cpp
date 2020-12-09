#include "dgemm.hpp"

int matrix_size;
int threads_num;
double **a;
double **b;
double **c;


double wtime()
{
  struct timeval t;
  gettimeofday(&t, NULL);

  return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}


void *pt(void *arg)
{
  int i, j, k, tid, portion_size, row_start, row_end;
  double sum;

  tid = *(int *)(arg);
  portion_size = matrix_size / threads_num;
  row_start = tid * portion_size;
  row_end = (tid == threads_num - 1) ? (matrix_size) : (row_start + portion_size);

  for (i = row_start; i < row_end; ++i) {
    for (j = 0; j < matrix_size; ++j) {
      sum = 0;
      for (k = 0; k < matrix_size; ++k) {
        sum += a[i][k] * b[k][j];
      }
      c[i][j] = sum;
    }
  }
}

void dgemmThreads()
{
  int i;
  pthread_t *threads = (pthread_t *)malloc(threads_num * sizeof(pthread_t));
  for (i = 0; i < threads_num; ++i) {
    int *tid;
    tid = (int *)malloc(sizeof(int));
    *tid = i;
    pthread_create(&threads[i], NULL, pt, (void *)tid);
  }

  for (i = 0; i < threads_num; ++i) {
    pthread_join(threads[i], NULL);
  }
}


int main(int argc, char const *argv[])
{
  if (argc != 3) {
    cout << "Launch format: ./[program_name] [matrix size] [number of threads]" << endl;
    return 1;
  }

  matrix_size = abs(atoi(argv[1]));
  threads_num = abs(atoi(argv[2]));

  //init matrix
  a = new double* [matrix_size];
  b = new double* [matrix_size];
  c = new double* [matrix_size];

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
    dgemmThreads();
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
