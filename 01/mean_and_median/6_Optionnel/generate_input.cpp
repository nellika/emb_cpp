//
// header-start
//////////////////////////////////////////////////////////////////////////////////
//
// \file      generate_input.cpp
//
// \brief     This file belongs to the C++ tutorial project
//
// \author    Nelli Nyisztor
//
//////////////////////////////////////////////////////////////////////////////////
// header-end
//


#include <fstream>
#include <random>
#include <cmath>
#include <iostream>

int main(int argc, char *argv[]) {
  int cnt = 100000;
  std::string file_name = "data";

  if (argc > 1) {
    cnt = std::stoi(argv[1]);
  }

  if (argc > 2) {
    file_name = argv[2];
  }

  std::string full_file_name = file_name + "_" + std::to_string(cnt) + ".txt";
  std::ofstream dataFile(full_file_name);

  std::random_device rd{};
  std::mt19937 gen{rd()};

  // mean of log10 values, stdev of log10 values
  std::normal_distribution<> d{3.24, 0.28};

  for (int n = 0; n < cnt; ++n) {
    // convert from normal to lognormal distro
    dataFile << pow(10, d(gen)) << std::endl;
  }

  std::cout << "File " << full_file_name << " has been created." << std::endl;

  dataFile.close();
}
