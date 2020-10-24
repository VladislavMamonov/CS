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
      data[i];

    t = (clock() - t) / CLOCKS_PER_SEC;
    bt->time_read += t;
  }

  bt->time_write /= Lnum;
  bt->time_read /= Lnum;

  delete [] data;
}


void bench_storage(BenchTime *bt, char *dir, int bytes, int Lnum = 1)
{
  double *data = new double[bytes / sizeof(double)];
  double t;
  double temp;

  FILE *storage_test;
  if ((storage_test = fopen(dir, "wb")) == NULL) {
    cout << "Test file creation error" << endl;
    return;
  }

  // запись
  for (long i = 0; i < Lnum; i++) {
    t = clock();
    if (fwrite(data, sizeof(double), bytes / sizeof(double), storage_test) != (bytes / sizeof(double))) {
      cout << "Error writing data to file" << endl;
      return;
    }
    t = (clock() - t) / CLOCKS_PER_SEC;
    bt->time_write += t;
  }

  fclose(storage_test);
  storage_test = fopen(dir, "rb");

  // чтение
  for (long i = 0; i < Lnum; i++) {
    t = clock();
    if (fread(data, sizeof(double), bytes / sizeof(double), storage_test) != (bytes / sizeof(double))) {
      cout << "Error reading data from file" << endl;
      return;
    }
    t = (clock() - t) / CLOCKS_PER_SEC;
    bt->time_read += t;
  }

  bt->time_write /= Lnum;
  bt->time_read /= Lnum;

  fclose(storage_test);
  delete [] data;
}


int main(int argc, char const *argv[])
{
  srand(time(NULL));
  long Lnum = 1;
  int bytes;
  struct BenchTime bt;
  // double FlopsPerSec;
  // double InsCount;

  if (argc < 4 || argc > 5) {
    cout << "Launch format: ./[program_name] [memory_subsystem] [data_block_size] [storage unit(optional)] [launch_number]" << endl;
    return 1;
  }

  if ((strcmp(argv[1], "RAM") != 0) && (strcmp(argv[1], "HDD") != 0) && (strcmp(argv[1], "SSD") != 0) && (strcmp(argv[1], "USB"))) {
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

  Lnum = atoi(argv[4]);

  if (argc == 4 || (strcmp(argv[3], "b") == 0))
    bytes = atoi(argv[2]);
  else if (argc > 4) {
    if (strcmp(argv[3], "kb") == 0)
      bytes = atoi(argv[2]) * 1024;
    if (strcmp(argv[3], "mb") == 0) {
      bytes = atoi(argv[2]) * 1024 * 1024;
    }
  }


  if (strcmp(argv[1], "RAM") == 0)
    bench_ram(&bt, bytes, Lnum);
  else if (strcmp(argv[1], "HDD") == 0)
    bench_storage(&bt, "storage_test.bin", bytes, Lnum);
  else if (strcmp(argv[1], "SSD") == 0)
    bench_storage(&bt, "/media/vladislav/8AB657BEB657A989/storage_test.bin", bytes, Lnum);
  else if (strcmp(argv[1], "USB") == 0)
    bench_storage(&bt, "/media/vladislav/ESD-USB/storage_test.bin", bytes, Lnum);


  ofstream BenchResults("BenchResults.csv");

  BenchResults << "Memory type: " << argv[1] << endl;
  BenchResults << "Block size: " << argv[2] << argv[3] << endl;
  BenchResults << "Element type: " << "double" << endl;
  BenchResults << "Buffer size: " << sizeof(double) << "b" << endl;
  BenchResults << "Launch number: " << argv[4] << endl;
  BenchResults << "Timer: clock" << endl;
  BenchResults << "Write time: " << endl;
  BenchResults << "Avg write time: " << bt.time_write << endl;
  BenchResults << "Write band width: " << (bytes / 1E+6) / bt.time_write;

  BenchResults.close();

  return 0;
}
