#include <filesystem>
#include <fstream>
#include <string>
#include <print>

int solve_part_1(std::string file) {
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

int modulo_100(int x) {
  return ((x % 100) + 100) % 100; // keep C++ mod in 0..100 (wrap from negative)
}

int solve_part_2(std::string file) {
  int result{};

  std::ifstream in{file};

  int current{50};
  std::print(" Points at: {}",current);

  std::string line{};
  while (std::getline(in,line)) {
    std::print("\nin:{}",line);

    int dir = (line[0]=='R')?1:-1;
    int val = std::stoi(line.substr(1));
    std::print(" -> dir:{} val:{}",dir,val);

    int delta = dir*val;
    int next = current + delta;

    int clicks{0};
    if (next > 0) {
      // We eneded up above zero. One click for each 100 interval
      clicks = next / 100;
      std::print("  >0 clicks: {}",clicks);
    }
    else if (next == 0) {
      // We ended up at zero
      result += 1;
      std::print("  ==0 clicks: {}",clicks);
    }
    else if (next < 0) {
      // We ended up below zero.
      clicks = ((-next + 100) / 100);
      if (current == 0) clicks += -1; // We already counted the zero 'crossing'
      std::print(" <0 clicks: {}",clicks);
    }

    result += clicks;

    current = modulo_100(next);

    std::print(" Points at: {} total clicks: {}",current,result);

  }

  std::print("\npassword: {}",result);

  return 0;

}


int day01() {
  // return solve_part_1("example01.txt");
  // return solve_part_1("day01.txt");
  return solve_part_2("example01.txt");
  // return solve_part_1("day01.txt");
}