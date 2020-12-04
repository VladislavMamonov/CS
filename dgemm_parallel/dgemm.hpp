#pragma once
#include <iostream>
#include <cmath>
#include <cstring>
#include <sys/time.h>
#include <omp.h>

using namespace std;


void init_matrix(int size, double **a, double **b, double **c);
void print_matrix(double **a, int n);

void dgemm(int size, double **a, double **b, double **c);
void dgemm_omp(int size, double **a, double **b, double **c, int threads_num);
