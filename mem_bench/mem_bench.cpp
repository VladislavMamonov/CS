#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <string>
#include <fstream>
#include <unistd.h>
#include <sys/time.h>

using namespace std;



struct BenchTime {
  long double time_write = 0;
  long double time_read = 0;
};



void bench_ram(BenchTime *bt, int bytes, long double *TimesWrite_iter, long double *TimesRead_iter, int Lnum = 1)
{
  struct timespec mt1, mt2;
  long double t;

  // запись
  for (long i = 0; i < Lnum; i++) {
    clock_gettime(CLOCK_REALTIME, &mt1);

    double *data = new double[bytes / sizeof(double)];
    for (int i = 0; i < bytes / 8; i++)
      data[i] = (double)rand();

    clock_gettime(CLOCK_REALTIME, &mt2);
    t = 1E+9 * (mt2.tv_sec - mt1.tv_sec) + (mt2.tv_nsec - mt1.tv_nsec);
    bt->time_write += t;
    TimesWrite_iter[i] = t;
    delete [] data;
  }

  // чтение
  for (long i = 0; i < Lnum; i++) {
    clock_gettime(CLOCK_REALTIME, &mt1);

    double *data = new double[bytes / sizeof(double)];
    for (int i = 0; i < bytes / 8; i++)
      data[i];

    clock_gettime(CLOCK_REALTIME, &mt2);
    t = 1E+9 * (mt2.tv_sec - mt1.tv_sec) + (mt2.tv_nsec - mt1.tv_nsec);
    bt->time_read += t;
    TimesRead_iter[i] = t;
    delete [] data;
  }

  bt->time_write /= Lnum;
  bt->time_read /= Lnum;
}



void bench_storage(BenchTime *bt, char *dir, int bytes, long double *TimesWrite_iter, long double *TimesRead_iter, int Lnum = 1)
{
  struct timespec mt1, mt2;
  double *data = new double[bytes / sizeof(double)];
  long double t;

  FILE *storage_test;
  if ((storage_test = fopen(dir, "wb")) == NULL) {
    cout << "Test file creation error" << endl;
    return;
  }

  // запись
  for (long i = 0; i < Lnum; i++) {
    clock_gettime(CLOCK_REALTIME, &mt1);
    if (fwrite(data, sizeof(double), bytes / sizeof(double), storage_test) != (bytes / sizeof(double))) {
      cout << "Error writing data to file" << endl;
      return;
    }
    clock_gettime(CLOCK_REALTIME, &mt2);
    t = 1E+9 * (mt2.tv_sec - mt1.tv_sec) + (mt2.tv_nsec - mt1.tv_nsec);
    bt->time_write += t;
    TimesWrite_iter[i] = t;
  }

  fclose(storage_test);
  storage_test = fopen(dir, "rb");

  // чтение
  for (long i = 0; i < Lnum; i++) {
    clock_gettime(CLOCK_REALTIME, &mt1);
    if (fread(data, sizeof(double), bytes / sizeof(double), storage_test) != (bytes / sizeof(double))) {
      cout << "Error reading data from file" << endl;
      return;
    }
    clock_gettime(CLOCK_REALTIME, &mt2);
    t = 1E+9 * (mt2.tv_sec - mt1.tv_sec) + (mt2.tv_nsec - mt1.tv_nsec);
    bt->time_read += t;
    TimesRead_iter[i] = t;
  }

  bt->time_write /= Lnum;
  bt->time_read /= Lnum;

  fclose(storage_test);
  delete [] data;
}



// double abs_error(double band_width)
// {
//   double frequency = 2133;
//   double bus_width = 64;
//   double channels = 1;
//
//   double reference_band_width = (frequency * bus_width * channels) / 8;
//   double abs_error = band_width - reference_band_width;
//
//   return abs_error;
// }



double relative_error_write(BenchTime *bt)
{
  double abs_err = 1;
  double relative_err = (abs_err / bt->time_write) * 100;
  return relative_err;
}



double relative_error_read(BenchTime *bt)
{
  double abs_err = 1;
  double relative_err = (abs_err / bt->time_read) * 100;
  return relative_err;
}



int main(int argc, char const *argv[])
{
  srand(time(NULL));
  long Lnum = 1;
  int bytes;
  struct BenchTime bt;

  if ((argc < 4 && argc != 2) || argc > 5) {
    cout << "Launch format: ./[program_name] [memory_subsystem] [data_block_size] [storage unit(optional)] [launch_number]" << endl;
    return -1;
  }

  if (argc > 2) {
    if ((strcmp(argv[1], "RAM") != 0) && (strcmp(argv[1], "HDD") != 0) && (strcmp(argv[1], "SSD") != 0) && (strcmp(argv[1], "USB"))) {
      cout << "Unknown memory subsystem" << endl;
      return -1;
    }
  }

  if (argc > 4) {
    if ((strcmp(argv[3], "b") != 0) && (strcmp(argv[3], "kb") != 0) && (strcmp(argv[3], "mb"))) {
      cout << "Unknown storage unit" << endl;
      return -1;
    }
    Lnum = atoi(argv[4]);
    if (argv[4] <= 0) Lnum = 1;
  }

  if (argc == 4) {
    Lnum = atoi(argv[3]);
    if (argv[3] <= 0)
      Lnum = 1;
  }

  if (argc == 2) Lnum = 20;

  long double TimesWrite_iter[Lnum];
  long double TimesRead_iter[Lnum];

  if (argc == 2) {
    if (strcmp(argv[1], "cache_L1") == 0) {
      bytes = 128 * 1024;   // L1 = 128kb
      bench_ram(&bt, bytes, TimesWrite_iter, TimesRead_iter, Lnum);
    } else if (strcmp(argv[1], "cache_L2") == 0) {
        bytes = 512 * 1024;   // L2 = 512kb
        bench_ram(&bt, bytes, TimesWrite_iter, TimesRead_iter, Lnum);
    } else if (strcmp(argv[1], "cache_L3") == 0) {
        bytes = 4 * 1024 * 1024;   // L3 = 4mb
        bench_ram(&bt, bytes, TimesWrite_iter, TimesRead_iter, Lnum);
    } else if (strcmp(argv[1], "cache_line") == 0) {
        bytes = 64;   // cache line = 64b
        bench_ram(&bt, bytes, TimesWrite_iter, TimesRead_iter, Lnum);
    } else {
      cout << "Unknown test" << endl;
      return -1;
    }
  }

  if (argc == 4 || (strcmp(argv[3], "b") == 0))
    bytes = atoi(argv[2]);
  else if (argc > 4) {
    if (strcmp(argv[3], "kb") == 0)
      bytes = atoi(argv[2]) * 1024;
    if (strcmp(argv[3], "mb") == 0) {
      bytes = atoi(argv[2]) * 1024 * 1024;
    }
  }

  if (argc > 2) {
    if (strcmp(argv[1], "RAM") == 0)
      bench_ram(&bt, bytes, TimesWrite_iter, TimesRead_iter, Lnum);
    else if (strcmp(argv[1], "HDD") == 0) {
      char *hdd_path = "storage_test";
      bench_storage(&bt, hdd_path, bytes, TimesWrite_iter, TimesRead_iter, Lnum);
      remove(hdd_path);
    }
    else if (strcmp(argv[1], "SSD") == 0) {
      char *ssd_path = "/media/vladislav/8AB657BEB657A989/storage_test.bin";
      bench_storage(&bt, ssd_path, bytes, TimesWrite_iter, TimesRead_iter, Lnum);
      remove(ssd_path);
    }
    else if (strcmp(argv[1], "USB") == 0) {
      char *usb_path = "/media/vladislav/ESD-USB/storage_test.bin";
      bench_storage(&bt, usb_path, bytes, TimesWrite_iter, TimesRead_iter, Lnum);
      remove(usb_path);
    }
  }

  double write_band_width = (bytes / (1024 * 1024)) / (bt.time_write / 1E+9);
  double read_band_width = (bytes / (1024 * 1024)) / (bt.time_read / 1E+9);

  ofstream BenchResults("BenchResults.csv");

  BenchResults << "Memory type: " << argv[1] << endl;
  BenchResults << "Block size: " << bytes << " bytes" << endl;
  BenchResults << "Element type: " << "double" << endl;
  BenchResults << "Buffer size: " << sizeof(double) << "b" << endl;
  BenchResults << "Launch number: " << Lnum << endl;
  BenchResults << "Timer: clock_gettime" << endl;
  BenchResults << "Write time: " << endl;
  for (int i = 0; i < Lnum; i++)
    BenchResults << "\t" << "Lnum" << "[" << i << "]: " << TimesWrite_iter[i] << " ns" << endl;
  BenchResults << "Avg write time: " << bt.time_write << " ns" << endl;
  BenchResults << "Write band width: " << write_band_width << " mb/s" << endl;
  BenchResults << "Absolute error(write): " << 1 << endl;
  BenchResults << "Relative error(write): " << relative_error_write(&bt) << "%" << endl;
  BenchResults << "Read time: " << endl;
  for (int i = 0; i < Lnum; i++)
    BenchResults << "\t" << "Lnum" << "[" << i << "]: " << TimesRead_iter[i] << " ns" << endl;
  BenchResults << "Avg read time: " << bt.time_read  << " ns" << endl;
  BenchResults << "Read band width: " << read_band_width << " mb/s" << endl;
  BenchResults << "Absolute error(read): " << 1 << endl;
  BenchResults << "Relative error(read): " << relative_error_read(&bt) << "%" << endl;


  BenchResults.close();
  return 0;
}
