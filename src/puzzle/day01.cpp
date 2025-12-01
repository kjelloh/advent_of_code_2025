#include <filesystem>
#include <fstream>
#include <string>
#include <print>

int solve(std::string file) {
  int result{};

  std::ifstream in{file};

  int location{50};
  std::print(" Points at: {}",location);

  std::string line{};
  while (std::getline(in,line)) {
    std::print("\nin:{}",line);

    int dir = (line[0]=='R')?1:-1;
    int val = std::stoi(line.substr(1));
    std::print(" -> dir:{} val:{}",dir,val);

    location = (location + dir*val) % 100;
    location = (location < 0)?location+100:location;

    std::print(" Points at: {}",location);

    result += (location==0)?1:0;

  }

  std::print("\npassword: {}",result);

  return 0;

}

int day01() {
  // return solve("example01.txt");
  return solve("day01.txt");
}