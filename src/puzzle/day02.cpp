#include <print>
#include <iostream>
#include <ranges>
#include <fstream>
#include <utility> // std::make_pair

struct Entry
{
    std::string s;
 
    friend std::istream &operator>>(std::istream &s, Entry &entry) {
        std::getline(s, entry.s,',');
        return s;
    }
}; // FileLine

int day02() {
  std::print("\nday02");
  std::ifstream in{"example02.txt"};
  auto parsed = 
      std::views::istream<Entry>(in)
    | std::views::transform([](auto const& entry){
        auto pos = entry.s.find('-');
        return std::make_pair(entry.s.substr(0,pos),std::stoi(entry.s.substr(pos+1)));
      });
  for (auto const& entry : parsed) {
    std::print("\n{} {} ",entry.first,entry.second);
    
  }
  return 0;
}