#include <string>
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
}; // Entry

int day02() {
  std::print("\nday02");
  std::ifstream in{"example02.txt"};
  auto parsed = 
      std::views::istream<Entry>(in)
    | std::views::transform([](auto const& entry){
        auto pos = entry.s.find('-');
        return std::make_pair(entry.s.substr(0,pos),entry.s.substr(pos+1));
      });
  for (auto const& entry : parsed) {
    std::print("\n{} {} ",entry.first,entry.second);

    auto [first,last] = std::make_pair(std::stoll(entry.first),std::stoll(entry.second));
    for (auto i= first;i<=last;++i) {
      if (i==first) std::print("\nfirst:{}",i);
      if (i==last) std::print("\nlast:{}",i);
    }

  }
  return 0;
}