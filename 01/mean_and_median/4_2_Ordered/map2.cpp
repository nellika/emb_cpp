//
// header-start
//////////////////////////////////////////////////////////////////////////////////
//
// \file      map2.cpp
//
// \brief     This file belongs to the C++ tutorial project
//
// \author    Nelli Nyisztor
//
//////////////////////////////////////////////////////////////////////////////////
// header-end
//

#include <string>
#include <map>
#include <fstream>
#include <iostream>

using std::string;

int main(int argc, char *argv[]) {
  string file_name{argv[1]};
  std::map<std::string, double> hash_map;
  std::multimap<double, std::string> hash_map_rev;
  std::ifstream fin(file_name, std::ios::in);

  string line;
  string hash_key;
  double hash_val;
  std::size_t end_of_hash, start_of_value;

  while (std::getline(fin, line)) {
    // split line into hash key and value
    end_of_hash = line.find_first_of(" ");
    start_of_value = line.find_last_of(" ") + 1;

    hash_key = line.substr(0, end_of_hash);
    hash_val = std::stod(line.substr(start_of_value));

    hash_map[hash_key] = hash_val;
    hash_map_rev.insert(std::pair<double, std::string>(hash_val, hash_key));
  }

  string qin;
  double upper, lower;
  const string break_kw = "END";
  do {
    std::cout << "query> ";
    std::cin >> qin;
    if (qin != break_kw) {
      // input entered for search
      if (qin.at(0) != '+') {
        // hash key based search
        auto search = hash_map.find(qin);
        if (search != hash_map.end()) {
          std::cout << "value[" << qin << "] = ";
          std::cout << hash_map[qin] << std::endl;
        } else {
          std::cerr << "ID " << qin << " does not exist... " << std::endl;
        }
      } else {
        // value based, range search
        // get lower and upper value bounds
        lower = std::stod(qin) * 0.99;
        upper = std::stod(qin) * 1.01;

        // get lower and upper map ID bounds
        auto itlow = hash_map_rev.lower_bound(lower);
        auto itup = hash_map_rev.upper_bound(upper);

        if ( itlow != itup ) {
          // range has items
          for ( auto it = itlow; it != itup; ++it ) {
            std::cout << "value[" << it->second << "] = ";
            std::cout << it->first << std::endl;
          }
        } else {
          // empty range
          std::cerr << "Range[" << lower << ", ";
          std::cerr << upper << "] is empty." << std::endl;
        }
      }
    } else {
      std::cout << "Bye..." << std::endl;
    }
  } while ( qin != break_kw );

  return 0;
}
