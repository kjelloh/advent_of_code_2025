#include <string>
#include <print>
#include <iostream>
#include <ranges>
#include <fstream>
#include <utility> // std::make_pair
#include <numeric> // std::accumulate,...

struct Entry
{
    std::string s;
 
    friend std::istream &operator>>(std::istream &s, Entry &entry) {
        std::getline(s, entry.s,',');
        return s;
    }
}; // Entry

bool is_invalid(auto id) {
  bool result{};

  auto s = std::to_string(id);

  if ((s.size() % 2) == 0) {
    // mirror possible
    auto second = s.size() / 2;
    result = (s.substr(0,second) == s.substr(second));
  }

  if (result) std::print(" invalid:{}",s);

  return result;
}

int day02() {
  std::print("\nday02");
  std::ifstream in{"example02.txt"};
  auto parsed = 
      std::views::istream<Entry>(in)
    | std::views::transform([](auto const& entry){
        auto pos = entry.s.find('-');
        return std::make_pair(entry.s.substr(0,pos),entry.s.substr(pos+1));
      });

  std::vector<size_t> invalids{};
  for (auto const& entry : parsed) {
    std::print("\n{} {} ",entry.first,entry.second);

    auto [first,last] = std::make_pair(std::stoll(entry.first),std::stoll(entry.second));
    for (auto i= first;i<=last;++i) {
      if (i==first) std::print("\nfirst:{}",i);
      if (i==last) std::print("\nlast:{}",i);

      if (is_invalid(i)) invalids.push_back(i);

    }
  }
  std::print("\ninvalids:{}",invalids.size());
  auto answer = std::accumulate(
     invalids.begin()
    ,invalids.end()
    ,0);

  std::print("\n\nANSWER:{}",answer);

  return 0;
}