//
// header-start
//////////////////////////////////////////////////////////////////////////////////
//
// \file      mean_and_median.c
//
// \brief     This file belongs to the C++ tutorial project
//
// \author    Bernard
//
// \copyright Copyright ng2goodies 2015
//            Distributed under the MIT License
//            See http://opensource.org/licenses/MIT
//
//////////////////////////////////////////////////////////////////////////////////
// header-end
//

//
// C version
// compiled with gcc 5.3.0
// gcc -O3 -o mean_and_median_c_version mean_and_median.c
//

#include <stdlib.h>
#include <stdio.h>


// Compare function for qsort()
// p and q are pointers to double 
//
int compare(const void *pd0, const void *pd1) {
  double d0 = *(double *)pd0;
  double d1 = *(double *)pd1;

  if (d0 > d1) {
    return 1;
  }
  if (d0 < d1) {
    return -1;
  }
  return 0;
}


// Read from a file a set of numbers
// compute a running mean value
// compute the median after sort
//
int main(int argc,char *argv[]) {
  char* file_name = argv[1];
  
  int res = 1000;
  double *buf = (double*)malloc(sizeof(double)*res);


  FILE *fin = fopen(file_name, "r");

  char  line[80];
  size_t   len;

  double mean = 0.0;
  int n = 0;
  while (fgets(line,80, fin) != NULL) {
    double d = strtod(line, NULL);
    if (n == res) {
      res += res;
      buf = (double*)realloc(buf,sizeof(double)*res);
    }
    buf[n++] = d;
    mean = (n==1) ? d : mean + (d - mean) / n;
  }
  
  qsort(buf, n, sizeof(double), compare);

  int mid = n / 2;
  double median = (n % 2) ? buf[mid] : 
                            (buf[mid - 1] + buf[mid]) / 2;

  printf("number of elements = %d, median = %g, mean = %g\n", n,
      median, mean);
 
  fclose(fin);
  free(buf);
}
