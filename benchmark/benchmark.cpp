#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <fstream>
#include <unistd.h>
#include <sys/time.h>

using namespace std;

#define FLOPS_SIN 4
#define FLOPS_LOG 60
#define FLOPS_POW 133


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


double benchmark_sin(const char *operand_type, double *times_iter, long Lnum = 1)
{
  double time = 0;
  double t;

  if (strcmp(operand_type, "double") == 0) {
    for (long i = 0; i < Lnum; i++) {
      t = clock();
      sin((double)rand());
      times_iter[i] = (clock() - t) / CLOCKS_PER_SEC;
      time += times_iter[i];
    }
  } else if (strcmp(operand_type, "float") == 0) {
    for (long i = 0; i < Lnum; i++) {
      t = clock();
      sin((float)rand());
      times_iter[i] = (clock() - t) / CLOCKS_PER_SEC;
      time += times_iter[i];
    }
  } else if (strcmp(operand_type, "long_double") == 0) {
    for (long i = 0; i < Lnum; i++) {
      t = clock();
      sin((long double)rand());
      times_iter[i] = (clock() - t) / CLOCKS_PER_SEC;
      time += times_iter[i];
    }
  } else return -1;

  time /= Lnum;

  return time;
}


double benchmark_log(const char *operand_type, double *times_iter, long Lnum = 1)
{
  double time = 0;
  double t;

  if (strcmp(operand_type, "double") == 0) {
    for (long i = 0; i < Lnum; i++) {
      t = clock();
      log((double)rand());
      times_iter[i] = (clock() - t) / CLOCKS_PER_SEC;
      time += times_iter[i];
    }
  } else if (strcmp(operand_type, "float") == 0) {
    for (long i = 0; i < Lnum; i++) {
      t = clock();
      log((float)rand());
      times_iter[i] = (clock() - t) / CLOCKS_PER_SEC;
      time += times_iter[i];
    }
  } else if (strcmp(operand_type, "long_double") == 0) {
    for (long i = 0; i < Lnum; i++) {
      t = clock();
      log((long double)rand());
      times_iter[i] = (clock() - t) / CLOCKS_PER_SEC;
      time += times_iter[i];
    }
  } else return -1;

  time /= Lnum;

  return time;
}


double benchmark_pow(const char *operand_type, double *times_iter, long Lnum = 1)
{
  double time = 0;
  double t;

  if (strcmp(operand_type, "double") == 0) {
    for (long i = 0; i < Lnum; i++) {
      t = clock();
      pow((double)rand(), 1000000000000);
      times_iter[i] = (clock() - t) / CLOCKS_PER_SEC;
      time += times_iter[i];
    }
  } else if (strcmp(operand_type, "float") == 0) {
    for (long i = 0; i < Lnum; i++) {
      t = clock();
      pow((float)rand(), 1000000000000);
      times_iter[i] = (clock() - t) / CLOCKS_PER_SEC;
      time += times_iter[i];
    }
  } else if (strcmp(operand_type, "long_double") == 0) {
    for (long i = 0; i < Lnum; i++) {
      t = clock();
      pow((long double)rand(), 1000000000000);
      times_iter[i] = (clock() - t) / CLOCKS_PER_SEC;
      time += times_iter[i];
    }
  } else return -1;

  time /= Lnum;

  return time;
}


double dispersion(double *times_iter, double avg_time, int Lnum)
{
  double dispersion = 0;

  for (int i = 0; i < Lnum; i++)
      dispersion += pow(times_iter[i] - avg_time, 2);

  dispersion /= (Lnum - 1);

  return dispersion;
}


double abs_error(double avg_time, double InsCount)
{
  double reference_time = InsCount / CLOCKS_PER_SEC;
  double abs_error = avg_time - reference_time;

  return abs_error;
}


double relative_error(double avg_time, double InsCount)
{
  double reference_time = InsCount / CLOCKS_PER_SEC;
  cout << "reference time: " << reference_time << endl;

  double abs_err = abs_error(avg_time, InsCount);
  double relative_err = (abs_err / reference_time) * 100;     //????

  return relative_err;
}


int main(int argc, char const *argv[])
{
  srand(time(NULL));
  long Lnum = 1;
  double time;
  double FlopsPerSec;
  double InsCount;

  if (argc != 4) {
    cout << "Launch format: ./[program_name] [test_name] [launch_number] [operand_type]" << endl;
    return 1;
  }

  if ((strcmp(argv[3], "double") != 0) && (strcmp(argv[3], "float") != 0) && (strcmp(argv[3], "long_double"))) {
    cout << "Unknown operand type" << endl;
    return 1;
  }

  Lnum = atoi(argv[2]);
  double *times_iter = new double[Lnum];

  if (strcmp(argv[1], "sin") == 0) {
    time = benchmark_sin(argv[3], times_iter, Lnum);
    FlopsPerSec = (FLOPS_SIN / 1e+9) / time;
    InsCount = FLOPS_SIN;
  }
  else if (strcmp(argv[1], "log") == 0) {
    time = benchmark_log(argv[3], times_iter, Lnum);
    FlopsPerSec = (FLOPS_LOG / 1e+9) / time;
    InsCount = FLOPS_LOG;
  }
  else if (strcmp(argv[1], "pow") == 0) {
    time = benchmark_pow(argv[3], times_iter, Lnum);
    FlopsPerSec = (FLOPS_POW / 1e+9) / time;
    InsCount = FLOPS_POW;
  }
  else {
    cout << "Unknown benchmark" << endl;
    return 1;
  }

  double disp = dispersion(times_iter, time, Lnum);

  cout << "dispersion: " << disp << endl;
  cout << "standard deviation: " << sqrt(disp) << endl;

  ofstream BenchResults("BenchResults.csv");

  BenchResults << "Pmodel: " << GetCpuModel() << endl;
  BenchResults << "Task: " << argv[1] << endl;
  BenchResults << "Operand type: " << argv[3] << endl;
  BenchResults << "Optimisations: " << GetOpt() << endl;
  BenchResults << "Number of launches: " << Lnum << endl;
  BenchResults << "Instruction count: " << InsCount << endl;
  BenchResults << "Average time: " << time << endl;
  BenchResults << "Absolute error: " << abs_error(time, InsCount) << endl;
  BenchResults << "Relative error: " << relative_error(time, InsCount) << endl;
  BenchResults << "Task performance: " << FlopsPerSec << " GFlops/sec";

  delete [] times_iter;
  BenchResults.close();
  return 0;
}
