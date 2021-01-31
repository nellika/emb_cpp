//
// header-start
//////////////////////////////////////////////////////////////////////////////////
//
// \file      map1.cpp
//
// \brief     This file belongs to the C++ tutorial project
//
// \author    Nelli Nyisztor
//
//////////////////////////////////////////////////////////////////////////////////
// header-end
//

#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>

using std::string;

int main(int argc, char *argv[]) {
  // tiny error check for missing file name
  if (argc < 2) {
    std::cout << "Trying to be tricky? ";
    std::cout << "Please provide a txt file with data..." << std::endl;
    return -1;
  }

  string file_name{argv[1]};
  std::unordered_map<std::string, double> hash_map;
  std::ifstream fin(file_name, std::ios::in);

  string line;

  while (std::getline(fin, line)) {
    // split line into hash key and value
    std::size_t end_of_hash = line.find_first_of(" ");
    std::size_t start_of_value = line.find_last_of(" ") + 1;

    string hash_key = line.substr(0, end_of_hash);
    double hash_val = std::stod(line.substr(start_of_value));

    hash_map[hash_key] = hash_val;
  }

  string qin;
  const string break_kw = "END";

  do {
    std::cout << "query> ";
    std::cin >> qin;
    if (qin != break_kw) {
      // input entered for search
      auto search = hash_map.find(qin);
      if (search != hash_map.end()) {
        // input found
        std::cout << "value[" << qin << "] = ";
        std::cout << hash_map[qin] << std::endl;
      } else {
        // not existing ID
        std::cout << "ID " << qin << " does not exist... " << std::endl;
      }
    } else {
      // END is entered > quit query input loop
      std::cout << "Bye..." << std::endl;
    }
  } while ( qin != break_kw );

  return 0;
}
