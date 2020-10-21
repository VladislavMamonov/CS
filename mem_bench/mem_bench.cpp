#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <unistd.h>
#include <sys/time.h>

using namespace std;


struct BenchTime {
  double time_write = 0;
  double time_read = 0;
};


void bench_ram(BenchTime *bt, int bytes, int Lnum = 1)
{
  double *data = new double[bytes / sizeof(double)];
  double t;
  double temp;

  // запись
  for (long i = 0; i < Lnum; i++) {
    t = clock();

    for (int i = 0; i < bytes / 8; i++)
      data[i] = (double)rand();

    t = (clock() - t) / CLOCKS_PER_SEC;
    bt->time_write += t;
  }

  // чтение
  for (long i = 0; i < Lnum; i++) {
    t = clock();

    for (int i = 0; i < bytes / 8; i++)
      temp = data[i];

    t = (clock() - t) / CLOCKS_PER_SEC;
    bt->time_read += t;
  }

  bt->time_write /= Lnum;
  bt->time_read /= Lnum;

  delete [] data;
}


void bench_hdd(BenchTime *bt, int bytes, int Lnum = 1)
{
  double *data = new double[bytes / sizeof(double)];
  FILE *hdd_test = fopen("hdd_test.bin", "w+b");
  double t;
  double temp;

  // запись
  for (long i = 0; i < Lnum; i++) {
    t = clock();

    fwrite(data, sizeof(double), bytes / sizeof(double), hdd_test);

    t = (clock() - t) / CLOCKS_PER_SEC;
    bt->time_write += t;
  }

  // чтение
  for (long i = 0; i < Lnum; i++) {
    t = clock();

    fread(data, sizeof(double), bytes / sizeof(double), hdd_test);

    t = (clock() - t) / CLOCKS_PER_SEC;
    bt->time_read += t;
  }

  bt->time_write /= Lnum;
  bt->time_read /= Lnum;

  delete [] data;
}


int main(int argc, char const *argv[])
{
  srand(time(NULL));
  long Lnum = 1;
  int bytes;
  struct BenchTime bt;
  // double time;
  // double FlopsPerSec;
  // double InsCount;

  if (argc < 4 || argc > 5) {
    cout << "Launch format: ./[program_name] [memory_subsystem] [data_block_size] [storage unit(optional)] [launch_number]" << endl;
    return 1;
  }

  if ((strcmp(argv[1], "RAM") != 0) && (strcmp(argv[1], "HDD") != 0) && (strcmp(argv[1], "flash"))) {
    cout << "Unknown memory subsystem" << endl;
    return 1;
  }

  if (argc > 4) {
    if ((strcmp(argv[3], "b") != 0) && (strcmp(argv[3], "kb") != 0) && (strcmp(argv[3], "mb"))) {
      cout << "Unknown storage unit" << endl;
      return 1;
    }
    if (argv[4] <= 0) Lnum = 1;
  }

  if (argc == 4) {
    if (argv[3] <= 0)
      Lnum = 1;
  }

  Lnum = atoi(argv[2]);

  if (argc == 4 || (strcmp(argv[3], "b") == 0))
    bytes = atoi(argv[2]);
  else if (argc > 4) {
    if (strcmp(argv[3], "kb") == 0)
      bytes = atoi(argv[2]) * 1024;
    if (strcmp(argv[3], "mb") == 0)
      bytes = atoi(argv[2]) * 1024 * 1024;
  }


  if (strcmp(argv[1], "RAM") == 0)
    bench_ram(&bt, bytes, Lnum);
  else if (strcmp(argv[1], "HDD") == 0)
    bench_hdd(&bt, bytes, Lnum);

  cout << bt.time_write << endl;
  cout << bt.time_read << endl;

  return 0;
}
