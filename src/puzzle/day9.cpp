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
  Tile(INT x,INT y) : x(x),y(y) {}
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

using Polygon = Model;

std::vector<FrameWithArea> to_areaed_frames(Polygon const& polygon) {
  auto V = polygon.size(); 
  std::vector<FrameWithArea> areaed_frames(V*(V-1)/2);
  unsigned ix{};
  for (int i=0;i<V;++i) {
    for (int j=i+1;j<V;++j) {
      auto frame = Frame{polygon[i],polygon[j]};
      auto area = spanned_area(frame.t1,frame.t2);
      areaed_frames[ix] = FrameWithArea(frame,area);
      ++ix;
    }
  }
  return areaed_frames;
}


std::optional<std::string> p1(PuzzleArgs puzzle_args) {

  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse(in);

  // Solve here
  UINT V = model.size();
  UINT candidate{};

  auto areaed_frames = to_areaed_frames(model);
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

INT signed_area(const Polygon& p) { 
    INT a = 0;
    for (size_t i = 0; i < p.size(); ++i) {
        const auto& cur = p[i];
        const auto& next = p[(i + 1) % p.size()];
        a += (cur.x * next.y - next.x * cur.y);
    }
    return a/2; 
}

bool rectilinear_polyregion_contains_frame(Polygon const& polygon,Frame const& frame) {
  auto [xt1,yt1] = frame.t1;
  auto [xt2,yt2] = frame.t2;
  auto x1 = std::min(xt1,xt2);
  auto x2 = std::max(xt1,xt2);
  auto y1 = std::min(yt1,yt2);
  auto y2 = std::max(yt1,yt2);
  Polygon rectangle{
     {x1,y1}
    ,{x1,y2}
    ,{x2,y2}
    ,{x2,y1}
  };

  // Not yet implemented

  return false;
}

std::optional<std::string> p2(PuzzleArgs puzzle_args) {

  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse(in);

  // Solve here
  const UINT V = model.size();
  UINT candidate{};

  auto polygon = model;

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
  for (int i=0;i<V;++i) {
    auto [x_first,y_first] = polygon[i];
    auto [x_second,y_second] = polygon[(i+1) % V];
    edges.push_back(Edge(Vertex(x_first,y_first),Vertex(x_second,y_second)));
  }

  // print
  aoc::print("\n{}",edges);
  aoc::print("\npolygon: E:{}",edges.size());
  auto E = edges.size();

  if (true) { 
    auto areaded_frames = to_areaed_frames(polygon);
    std::ranges::sort(areaded_frames,[](auto const& lhs,auto const& rhs){
      return (lhs.area > rhs.area);
    });
    std::set<INT> histogram{};
    int cap{1000};
    for (auto const& af : areaded_frames) {
      // Pick the first valid one
      // aoc::print("\n{:3} {}",cap,af.area);
      // if (--cap ==0 ) break;
      histogram.insert(af.area/1000000);
    }
    aoc::print("\nhistogram:{}",histogram.size()); // 100 000->39271, 1 000 000 -> 4721, 10 000 000 -> 477
  }

  if (false) {
    std::vector<Edge> horizontals{};
    std::vector<Edge> verticals{};
    for (auto const& edge : edges) {
      auto [x1,y1] = edge.first;
      auto [x2,y2] = edge.second;
      if (x2-x1 == 0) verticals.push_back(edge);
      if (y2-y1 == 0) horizontals.push_back(edge);
    }
    if (verticals.size() + horizontals.size() != E) 
      return std::format(
        "Failed to account for all edges {}",E);
    auto M = horizontals.size();
    auto N = verticals.size();
    aoc::print("\nM:{} N:{}",M,N);
  }
  if (false) {
    std::vector<INT> xs{};
    std::vector<INT> ys{};
    for (auto const& p : polygon) {
      xs.push_back(p.x);
      ys.push_back(p.y);
    }
    auto make_unique = [](auto& vec) {
        std::ranges::sort(vec);
        vec.erase(std::ranges::unique(vec).begin(), vec.end());
    };

    make_unique(xs);
    make_unique(ys);
    aoc::print("\nxs:{}",xs);
    aoc::print("\nys:{}",ys);
    auto M = xs.size();
    auto N = ys.size();
    aoc::print("\nM:{} N:{}",M,N);

    std::vector<std::vector<bool>> z(N,std::vector<bool>(M)); // MxN bool matrix

    auto to_compressed = [](auto const& coords, INT value)  { 
        return std::ranges::lower_bound(coords, value) - coords.begin();
    };

    for (size_t i = 0; i < polygon.size(); ++i) {
      // start - end
      auto const& p1 = polygon[i]; // begin
      auto const& p2 = polygon[(i+1) % V]; // end

      // Compressed
      auto cx1 = to_compressed(xs, p1.x);
      auto cy1 = to_compressed(ys, p1.y);
      auto cx2 = to_compressed(xs, p2.x);
      auto cy2 = to_compressed(ys, p2.y);
      
      // Normalise
      if (cx2<cx1) std::swap(cx1,cx2);
      if (cy2<cy1) std::swap(cy1,cy2);

      // Mark in compressed
      if (cy1==cy2) for (auto cx=cx1;cx<cx2;++cx) z[cy1][cx] = true;
      if (cx1==cx2) for (auto cy=cy1;cy<cy2;++cy) z[cy][cx1] = true; 
    }

    // print z
    aoc::print("\n{}",z);

  }

  if (false) {
    // All possible picks for rectangle
    for (size_t i=0;i<V;++i) {
      for (size_t j=i+1;j<V;++j) {
        if (i==j) continue;
        auto t1 = polygon[i];
        auto t2 = polygon[j];
        if (t1.x == t2.x) continue;
        if (t1.y == t2.y) continue;

        Frame frame(t1,t2);

        if (rectilinear_polyregion_contains_frame(polygon,frame)) {
          auto a = spanned_area(frame.t1,frame.t2);
          candidate = std::max(candidate,a);
        }

      }
    }
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
