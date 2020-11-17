#pragma once
#include <iostream>
#include <cmath>
#include <sys/time.h>

using namespace std;


void init_matrix(int size, double **a, double **b, double **c);
void print_matrix(double **a, int n);

void dgemm(int size, double **a, double **b, double **c);
void dgemm_opt_1(int size, double **a, double **b, double **c);
void dgemm_opt_2(double *a, double *b, double *c, int n);
