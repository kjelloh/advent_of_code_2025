#include "aoc25.h"
#include <string>
#include <print>
#include <iostream>
#include <fstream>
#include <optional>
#include <algorithm>
#include <set>
#include <map>
#include <sstream>

using INT = int64_t;
using UINT = uint64_t;

using Vertex = std::pair<INT,INT>; 
using Edge = std::pair<Vertex,Vertex>;

class Tile {
public:
  INT x;
  INT y;
  Tile() {}
  Tile(std::string const& s) {
    std::istringstream iss{s};
    std::string number;
    std::getline(iss,number,',');
    x = std::stoll(number);
    std::getline(iss,number,',');
    y = std::stoll(number);    
  }
};

std::string to_string(Tile const& tile) {
  return std::format("{},{}",tile.x,tile.y);
}

struct Frame {
  Tile t1;
  Tile t2;
};

struct FrameWithArea {
  Frame frame{};
  UINT area{};
};

using Model = std::vector<Tile>;

Model parse(std::istream& in) {
  Model model{};

  std::string entry;
  int ix{0};
  while (std::getline(in, entry)) {
    aoc::print("\nin[{:4}][0..{:3}]: '{}' ", ix++,entry.size()-1,entry);
    model.push_back(Tile(entry));
  }
  return model;
}

UINT spanned_area(Tile t1,Tile t2) {
  return ((std::abs(t2.x-t1.x) +1)* (std::abs(t2.y-t1.y) +1));
}

std::optional<std::string> p1(PuzzleArgs puzzle_args) {

  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse(in);

  // Solve here
  UINT V = model.size();
  UINT candidate{};

  std::vector<FrameWithArea> areaed_frames(V*(V-1)/2);
  unsigned ix{};
  for (int i=0;i<V;++i) {
    for (int j=i+1;j<V;++j) {
      auto frame = Frame{model[i],model[j]};
      auto area = spanned_area(frame.t1,frame.t2);
      areaed_frames[ix] = FrameWithArea(frame,area);
      ++ix;
    }
  }

  ix = 0;
  for (auto const& [frame,area] : areaed_frames) {
    aoc::print(
       "\n{}: {} x {} = {}"
      ,ix++ 
      ,to_string(frame.t1)
      ,to_string(frame.t2)
      ,area);
  }

  auto iter = std::ranges::max_element(areaed_frames,[](auto const& lhs,auto const& rhs){
    return lhs.area < rhs.area;
  });

  UINT max{};
  for (auto const& [frame,area] : areaed_frames) {
    max = std::max(max,area);
  }

  aoc::print("\nmax:{}",max);

  aoc::print(
     "\nmax:{} for {} - {}"
    ,iter->area
    ,to_string(iter->frame.t1)
    ,to_string(iter->frame.t2));

  // max:4762764240 for 84968,83848 - 15781,15007
  // (15781-84968 + 1) * (15007-83848 +1)
  // -69186 * -68840 = 4 762 764 240 (order of 10^9)
  // 2^64 = 1 844 674 407 370 955 xxx (order of 10^19)
  // 2^32 = 4 294 967 296 (order of 10^9)

  candidate = iter->area; // 4762902267,4762764240 not correct
                          //            4763040296

  // return {};
  // return std::format("Not yet fully implemented");
  return std::format("{}",candidate);

} // p1

using Polygon = Model;

double signed_area(const Polygon& p) {
    double a = 0.0;
    for (size_t i = 0; i < p.size(); ++i) {
        const auto& cur = p[i];
        const auto& next = p[(i + 1) % p.size()];
        a += (cur.x * next.y - next.x * cur.y);
    }
    return 0.5 * a;
}

std::optional<std::string> p2(PuzzleArgs puzzle_args) {

  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse(in);

  // Solve here
  const UINT V = model.size();
  UINT candidate{};

  auto polygon = model;
  auto N = polygon.size();

  auto a = signed_area(polygon);
  aoc::print("\nsigned area:{}",a);
  if (a<0) {
    aoc::print(" -> Reverse for counter clockwise vertex order");
    std::ranges::reverse(polygon); 
  }
  else {
    aoc::print(" -> Counter clockwise vertices OK");
  }

  std::vector<Edge> edges{};
  for (int i=0;i<N;++i) {
    auto [x_first,y_first] = polygon[i];
    auto [x_second,y_second] = polygon[(i+1) % N];
    edges.push_back(Edge(Vertex(x_first,y_first),Vertex(x_second,y_second)));
  }

  // print
  aoc::print("\n{}",edges);

  aoc::print("\npolygon: E:{}",model.size());

  if (false) {
    // Cooridnate compression

    // Compress ranges to compressed matrix
    std::vector<INT> xs(V);
    std::vector<INT> ys(V);
    for (size_t i=0;i<V;++i) {
      xs[i] = model[i].x;
      ys[i] = model[i].y;
    }

    // Arrange boundaries in order
    std::ranges::sort(xs);
    std::ranges::sort(ys);

    // Deduplicate boundaries
    xs.erase(
      std::unique(xs.begin(),xs.end()) // return iter to tail of duplicates
      ,xs.end());
    ys.erase(
      std::unique(ys.begin(),ys.end())
      ,ys.end()
    );

    // Actual compressed sizes
    const UINT X = xs.size();
    const UINT Y = ys.size();

    // Create otside (+1 size) BOUND
    const std::pair<INT,INT> X_BOUND(xs.front()-1,xs.back()+1);
    const std::pair<INT,INT> Y_BOUND(ys.front()-1,ys.back()+1);

    // Compress ranges to indices idx
    std::vector<std::pair<INT,INT>> compressed_x(X+1);
    compressed_x[0] = std::make_pair(X_BOUND.first,xs[0]);
    compressed_x[X] = std::make_pair(xs[X-1],X_BOUND.second);
    std::vector<std::pair<INT,INT>> compressed_y(Y+1);
    compressed_y[0] = std::make_pair(Y_BOUND.first,ys[0]);
    compressed_y[Y] = std::make_pair(ys[Y-1],Y_BOUND.second);
    for (size_t i=1;i<X;++i) {
      compressed_x[i] = std::make_pair(xs[i-1],xs[i]);
    }
    for (size_t i=1;i<Y;++i) {
      compressed_y[i] = std::make_pair(ys[i-1],ys[i]);
    }

    const UINT CX = compressed_x.size();
    const UINT CY = compressed_y.size();

    std::vector<std::vector<char>> compressed_yx(
      CY
      ,std::vector<char>(CX)
    );

    auto x_to_idx = [&xs](unsigned x) {
      auto it = std::lower_bound(xs.begin(), xs.end(), x); // any x on actual grid
      size_t idx = it - xs.begin();
    };
    auto y_to_idy = [&ys](unsigned y) {
      auto it = std::lower_bound(ys.begin(), ys.end(), y);
      size_t idx = it - ys.begin();  // gives index of interval [xs[idx-1], xs[idx]) that contains x  
    };

    // Fill the compressed grid (mark compressed[id] -> grid coord ranges as inside/outside)

  }

  // return {};
  return std::format("Not yet fully implemented?");
  // return std::format("{}",candidate);
} // p2

std::optional<std::string> day(PuzzleArgs puzzle_args) {
  aoc::print(
    "\nday part:{} debug:{}"
    ,puzzle_args.meta().m_part
    ,puzzle_args.meta().m_debug);
  
  switch (puzzle_args.meta().m_part) {
    case 1: {
      return p1(puzzle_args);
    } break;
    case 2:
      return p2(puzzle_args);
      break;
  }
  return std::nullopt;
}
