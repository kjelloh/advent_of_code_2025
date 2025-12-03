#include <filesystem>
#include <fstream>
#include <string>
#include <print>

// Mathematical mod using C/C++ signed reminder operator '%'
int mod(int x, int N) {
    int r = x % N; // signed reminder
    return (r < 0) ? r + N : r; // compensate for sign
}

std::string solve_part_1(std::filesystem::path in_file_path) {
  int result{};

  std::ifstream in{in_file_path};

  int location{50};
  std::print(" Points at: {}",location);

  std::string line{};
  while (std::getline(in,line)) {
    std::print("\nin:{}",line);

    int dir = (line[0]=='R')?1:-1;
    int val = std::stoi(line.substr(1));
    std::print(" -> dir:{} val:{}",dir,val);

    location = mod(location + dir*val,100);

    std::print(" Points at: {}",location);

    result += (location==0)?1:0;

  }

  return std::format("password: {}",result);


}

std::string solve_part_2(std::string in_file_path) {
  int result{};

  std::ifstream in{in_file_path};

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
      clicks = 1;
      std::print("  ==0 clicks: {}",clicks);
    }
    else if (next < 0) {
      // We ended up below zero.
      clicks = ((-next + 100) / 100);
      if (current == 0) clicks -= 1; // We already counted the zero 'crossing'
      std::print(" <0 clicks: {}",clicks);
    }

    result += clicks;

    current = mod(next,100); // 'Wrap' to nob markings 0..99

    std::print(" Points at: {} total clicks: {}",current,result);

  }

  return std::format("password: {}",result);
}


std::string day(std::filesystem::path file_path) {
  // return solve_part_1("example01.txt");
  // return solve_part_1("day01.txt");
  // return solve_part_2("example01.txt");
  return solve_part_2(file_path.filename());
}