#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <fstream>
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


double benchmark_atan2(long Lnum = 1)
{
  double time = 0;
  double t;

  for (long i = 0; i < Lnum; i++) {
    t = clock();
    atan2(rand(), rand());
    time += (clock() - t) / CLOCKS_PER_SEC;
  }

  time /= Lnum;

  return time;
}


double benchmark_cbrt(long Lnum = 1)
{
  double time = 0;
  double t;

  for (long i = 0; i < Lnum; i++) {
    t = clock();
    cbrt(rand());
    time += (clock() - t) / CLOCKS_PER_SEC;
  }

  time /= Lnum;

  return time;
}


double benchmark_hypot(long Lnum = 1)
{
  double time = 0;
  double t;

  for (long i = 0; i < Lnum; i++) {
    t = clock();
    hypot(rand(), rand());
    time += (clock() - t) / CLOCKS_PER_SEC;
  }

  time /= Lnum;

  return time;
}


int main(int argc, char const *argv[])
{
  srand(time(NULL));
  long Lnum = 1;
  double time;

  if (argc == 1) {
    cout << "Launch format: ./[program_name] [test_name] [launch_number]" << endl;
    return 1;
  }

  if (argc == 3)
    Lnum = atoi(argv[2]);

  if (strcmp(argv[1], "atan2") == 0)
    time = benchmark_atan2(Lnum);
  else if (strcmp(argv[1], "cbrt") == 0)
    time = benchmark_cbrt(Lnum);
  else if (strcmp(argv[1], "hypot") == 0)
    time = benchmark_hypot(Lnum);
  else {
    cout << "Unknown benchmark" << endl;
    return 1;
  }

  ofstream BenchResults("BenchResults.csv");

  BenchResults << "Pmodel - " << GetCpuModel() << endl;
  BenchResults << "Task - " << argv[1] << endl;
  BenchResults << "Number of launches - " << Lnum << endl;
  BenchResults << "Average time - " << time << endl;

  BenchResults.close();
  return 0;
}
