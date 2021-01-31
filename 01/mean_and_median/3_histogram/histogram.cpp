//
// header-start
//////////////////////////////////////////////////////////////////////////////////
//
// \file      histogram.cpp
//
// \brief     This file belongs to the C++ tutorial project
//
// \author    Nelli Nyisztor
//
//////////////////////////////////////////////////////////////////////////////////
// header-end
//


#include <math.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

using std::string;
using std::vector;

int main(int argc, char *argv[]) {
  // tiny error check for missing file name
  if (argc < 2) {
    std::cout << "Trying to be tricky? ";
    std::cout << "Please provide a txt file with data..." << std::endl;
    return -1;
  }

  string file_name{argv[1]};
  //  vector containing actual values from txt
  vector<double> buf;
  //  vector containing the number of items in a given category
  vector<double> cnt;
  std::ifstream fin(file_name, std::ios::in);

  string line;
  auto mean = 0.0;

  while (std::getline(fin, line)) {
    auto d = std::stod(line);
    buf.push_back(d);
    mean = (buf.size() == 1) ? d : mean + (d - mean) / buf.size();
  }

  std::sort(buf.begin(), buf.end());

  auto mid = buf.size() / 2;
  double median = (buf.size() % 2) ? buf[mid] : (buf[mid - 1] + buf[mid]) / 2;

  std::cout << "number of elements = " << buf.size()
            << ", median = " << median
            << ", mean = " << mean << std::endl;

  int max_axis = floor(buf[buf.size() - 1] / 100) + 1;

  //  initialize nr of elements vector
  for (int n = 0; n < max_axis; n++) {
    cnt.push_back(0);
  }

  //  start the count for categories per element
  for (size_t i = 0; i < buf.size(); i++) {
    cnt[floor(buf[i] / 100)] += 1;
  }

  //  get max value to calculate proportional star count
  double max_cnt = *std::max_element(std::begin(cnt), std::end(cnt));

  //  print each row
  for (int n = 0; n < max_axis; n++) {
    std::cout << std::right << std::setw(6) << n * 100;
    std::cout << std::right << std::setw(8) << cnt[n] << " ";

    // print necessary stars
    std::cout << std::string(floor(60/ (max_cnt / cnt[n])), '*') << std::endl;
  }

  return 0;
}
