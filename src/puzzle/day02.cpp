#include <string>
#include <print>
#include <iostream>
#include <ranges>
#include <fstream>
#include <utility> // std::make_pair
#include <numeric> // std::accumulate,...
#include <set>

namespace aoc {

  // function compute_failure_function(s):
  //     n = length of s
  //     failure = array of size n, initialized to 0
      
  //     j = 0  // length of previous longest prefix-suffix match
      
  //     for i from 1 to n-1:
  //         // Try to extend the match
  //         while j > 0 and s[i] != s[j]:
  //             j = failure[j - 1]  // fall back using previous failure values
          
  //         // If characters match, increment the match length
  //         if s[i] == s[j]:
  //             j = j + 1
          
  //         failure[i] = j
      
  //     return failure

  // Knuth–Morris–Pratt algorithm (https://en.wikipedia.org/wiki/Knuth–Morris–Pratt_algorithm)
  std::vector<size_t> to_kmp_failure_v(std::string_view const s) {
    std::vector<size_t> result{};
    return result;
  }

}

struct Entry
{
    std::string s;
 
    friend std::istream &operator>>(std::istream &s, Entry &entry) {
        std::getline(s, entry.s,',');
        return s;
    }
}; // Entry

// Trie if provided id is made up of two repeating patterns
bool is_two_repeats(auto id) {
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

// returns true if provided id is made up of any length repeats 1..1/2 length
bool contains_repeats(auto id) {
  bool result{};

  auto s = std::to_string(id);


  for (size_t i=1;i<s.size();++i) {

    if ((s.size() % i) == 0) {
      // splits into i parts
      auto count = s.size() / i;
      std::set<std::string> parts_set{};
      for (int j=0;j<count;++j) {
        parts_set.insert(s.substr(i*j,i));
      }
      if (parts_set.size() == 1) {
        result = true;
        break;
      }
    }
  }

  if (result) std::print(" invalid:{}",s);

  return result;
}

int day() {
  std::print("\nday02");
  std::ifstream in{"in.txt"};
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

      if (contains_repeats(i)) invalids.push_back(i);

    }
  }
  std::print("\ninvalids:{}",invalids.size());
  auto answer = std::accumulate(
     invalids.begin()
    ,invalids.end()
    ,size_t{});

  std::print("\n\nANSWER:{}",answer);

  return 0;
}