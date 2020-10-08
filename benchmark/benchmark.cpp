#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <fstream>
#include <unistd.h>
#include <time.h>

using namespace std;

string GetCpuModel()
{
  ifstream cpuinfo("/proc/cpuinfo");
  string cpu_model;

  for (int i = 0; i < 5; i++)
    getline(cpuinfo, cpu_model);

  cpu_model = cpu_model.substr(13);

  cpuinfo.close();
  return cpu_model;
}


string GetOpt()
{
  ifstream makefile("Makefile");
  string opt;

  for (int i = 0; i < 3; i++)
    makefile >> opt;

  makefile.close();
  return opt;
}


double benchmark_atan2(const char *operand_type, long Lnum = 1)
{
  double time = 0;
  double t;

  if (strcmp(operand_type, "double") == 0) {
    for (long i = 0; i < Lnum; i++) {
      t = clock();
      atan2((double)rand(), (double)rand());
      time += (clock() - t) / CLOCKS_PER_SEC;
    }
  } else if (strcmp(operand_type, "float") == 0) {
    for (long i = 0; i < Lnum; i++) {
      t = clock();
      atan2((float)rand(), (float)rand());
      time += (clock() - t) / CLOCKS_PER_SEC;
    }
  } else if (strcmp(operand_type, "long double") == 0) {
    for (long i = 0; i < Lnum; i++) {
      t = clock();
      atan2((long double)rand(), (long double)rand());
      time += (clock() - t) / CLOCKS_PER_SEC;
    }
  } else return -1;

  time /= Lnum;

  return time;
}


double benchmark_cbrt(const char *operand_type, long Lnum = 1)
{
  double time = 0;
  double t;

  if (strcmp(operand_type, "double") == 0) {
    for (long i = 0; i < Lnum; i++) {
      t = clock();
      cbrt((double)rand());
      time += (clock() - t) / CLOCKS_PER_SEC;
    }
  } else if (strcmp(operand_type, "float") == 0) {
    for (long i = 0; i < Lnum; i++) {
      t = clock();
      cbrt((float)rand());
      time += (clock() - t) / CLOCKS_PER_SEC;
    }
  } else if (strcmp(operand_type, "long double") == 0) {
    for (long i = 0; i < Lnum; i++) {
      t = clock();
      cbrt((long double)rand());
      time += (clock() - t) / CLOCKS_PER_SEC;
    }
  } else return -1;

  time /= Lnum;

  return time;
}


double benchmark_hypot(const char *operand_type, long Lnum = 1)
{
  double time = 0;
  double t;

  if (strcmp(operand_type, "double") == 0) {
    for (long i = 0; i < Lnum; i++) {
      t = clock();
      hypot((double)rand(), (double)rand());
      time += (clock() - t) / CLOCKS_PER_SEC;
    }
  } else if (strcmp(operand_type, "float") == 0) {
    for (long i = 0; i < Lnum; i++) {
      t = clock();
      hypot((float)rand(), (float)rand());
      time += (clock() - t) / CLOCKS_PER_SEC;
    }
  } else if (strcmp(operand_type, "long_double") == 0) {
    for (long i = 0; i < Lnum; i++) {
      t = clock();
      hypot((long double)rand(), (long double)rand());
      time += (clock() - t) / CLOCKS_PER_SEC;
    }
  } else return -1;

  time /= Lnum;

  return time;
}


int main(int argc, char const *argv[])
{
  srand(time(NULL));
  long Lnum = 1;
  double time;

  if (argc != 4) {
    cout << "Launch format: ./[program_name] [test_name] [launch_number] [operand_type]" << endl;
    return 1;
  }

  if ((strcmp(argv[3], "double") != 0) && (strcmp(argv[3], "float") != 0) && (strcmp(argv[3], "long_double"))) {
    cout << "Unknown operand type" << endl;
    return 1;
  }

  Lnum = atoi(argv[2]);

  if (strcmp(argv[1], "atan2") == 0)
    time = benchmark_atan2(argv[3], Lnum);
  else if (strcmp(argv[1], "cbrt") == 0)
    time = benchmark_cbrt(argv[3], Lnum);
  else if (strcmp(argv[1], "hypot") == 0)
    time = benchmark_hypot(argv[3], Lnum);
  else {
    cout << "Unknown benchmark" << endl;
    return 1;
  }

  ofstream BenchResults("BenchResults.csv");

  BenchResults << "Pmodel - " << GetCpuModel() << endl;
  BenchResults << "Task - " << argv[1] << endl;
  BenchResults << "Operand type - " << argv[3] << endl;
  BenchResults << "Optimisations - " << GetOpt() << endl;
  BenchResults << "Number of launches - " << Lnum << endl;
  BenchResults << "Average time - " << time << endl;

  BenchResults.close();
  char *arg_execv[] = { "instruction_counter", 0 };
  execv(arg_execv[0], arg_execv);
  return 0;
}
