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
#include <unordered_set>
#include <z3++.h>
#include <numeric> // std::accumulate,...

using INT = int64_t;
using UINT = uint64_t;

using Lights = std::string;
using Button = std::vector<unsigned>;
using Buttons = std::vector<Button>;
using Joltage = std::vector<unsigned>;

struct Machine {
  Lights lights;
  std::vector<Button> buttons;
  Joltage joltage;

};

std::tuple<std::string_view,std::string_view,std::string_view> to_sections_svs(std::string_view sv) {
  // [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
  //       ^                               ^
  auto pos1 = sv.find(']')+1;
  auto pos2 = sv.find('{');  
  return {
    sv.substr(0,pos1)
    ,sv.substr(pos1,pos2-pos1 -1)
    ,sv.substr(pos2)};  
}

std::vector<std::string_view> to_buttons_svs(std::string_view sv) {
  // ' (3) (1,3) (2) (2,3) (0,2) (0,1)'
  //      ^     ^   ^     ^     ^
  std::vector<std::string_view> result{};
  size_t begin{0};
  aoc::print("\nto_buttons_svs:");
  size_t pos{0};
  while (pos < sv.size()) {
    auto end = sv.find(')',pos);
    result.push_back(sv.substr(pos,end-pos+1));
    aoc::print(" '{}'",result.back());
    pos = end+1;
  }
  return result;
}

Lights to_lights(std::string_view sv) {
  // [.##.]
  return Lights{sv.substr(1,sv.size()-2)};
}

Button to_button(std::string_view sv) {
  // ' (1,3)'
  // ' (3)'
  Button result{};
  aoc::print("\nto_button:");
  auto begin = sv.find('(') + 1;
  auto find_end = [&sv](size_t begin){
    auto pos = sv.find(',',begin);
    if (pos == std::string_view::npos) pos = sv.find(')',begin);
    return pos;
  };
  auto end = find_end(begin);
  while (begin < sv.size()) {
    try {
      std::string val(sv.substr(begin,end-begin));
      aoc::print(" '{}'",val);
      result.push_back(std::stoi(val));
    }
    catch (std::exception const& e) {
      std::print("\nto_button: Excpetion - {}",e.what());
    }
    begin = end+1;
    end = find_end(begin);
  }
  return result;
}

Buttons to_buttons(std::string_view sv) {
  Buttons result{};
  auto svs = to_buttons_svs(sv);
  for (auto const& sv : svs) {
    result.push_back(to_button(sv));
  }
  return result;
}

Joltage to_joltage(std::string_view sv) {
  Joltage result{};
  aoc::print("\nto_joltage:");
  auto begin = sv.find('{') + 1;
  auto find_end = [&sv](size_t begin){
    auto pos = sv.find(',',begin);
    if (pos == std::string_view::npos) pos = sv.find('}',begin);
    return pos;
  };
  auto end = find_end(begin);
  while (begin < sv.size()) {
    try {
      std::string val(sv.substr(begin,end-begin));
      aoc::print(" '{}'",val);
      result.push_back(std::stoi(val));
    }
    catch (std::exception const& e) {
      std::print("\to_joltage: Excpetion - {}",e.what());
    }
    begin = end+1;
    end = find_end(begin);
  }
  return result;
}

Machine to_machine(std::string_view sv) {
  auto const& [lights,buttons,joltage] = to_sections_svs(sv);
  aoc::print(
    "\nto_machine '{}' '{}' '{}'"
    ,lights
    ,buttons
    ,joltage);
  return Machine {
    .lights = to_lights(lights)
    ,.buttons = to_buttons(buttons)
    ,.joltage = to_joltage(joltage)
  };
}

using Model = std::vector<Machine>;
Model parse(std::istream& in) {
  Model model{};

  std::string entry;
  int ix{0};
  while (std::getline(in, entry)) {
    aoc::print("\nin[{:4}][0..{:3}]: '{}' ", ix++,entry.size()-1,entry);
    model.push_back(to_machine(entry));
    aoc::print(
       "\nmachine: lights:'{}'"
      ,model.back().lights);
  }
  return model;
}

std::string to_string(Button const& button) {
  std::string result{};
  result.push_back('(');
  bool first(true);
  for (auto lx : button) {
    if (!first) {
      result.push_back(',');
    }
    result += std::to_string(lx);
    first = false;
  }
  result.push_back(')');
  return result;
}

std::string to_string(Buttons const& buttons) {
  std::string result{};
  bool first(true);
  for (auto const& button : buttons) {
    if (!first) {
      result.push_back(' ');
    }
    first = false;
  }
  return result;
}

Lights press(Button const& button,Lights state) {
  Lights result(state);
  aoc::print(
     "\npress({},'{}')"
    ,to_string(button)
    ,state);
  for (auto lx : button) {
    bool l(result[lx] == '#');
    l = not l;
    result[lx] = (l)?'#':'.';
  }
  aoc::print(" -> '{}'",result);
  return result;
}


Lights press(unsigned bx,Lights state,Machine const& machine) {
  return press(machine.buttons[bx],state);
}

using Expected = std::vector<unsigned>;

Expected press(Button const& button,Expected state,bool is_part2 = false) {
  Expected result(state);
  // Press button
  for (auto bx : button) {
    ++result[bx]; 
    if (!is_part2) result[bx] %= 2; // part 1
  }
  return result;
}

struct State {
  Expected value;
  size_t press_count{};
  State(size_t size) : value(size,0) {}
  bool operator==(Expected const& expected) {
    return value == expected;
  }
};

Expected to_expected(Lights const& lights) {
  Expected result(lights.size(),0);
  for (size_t i=0;i<lights.size();++i) {
    result[i] = (lights[i]=='#')?1:0;
  }
  return result;
}

INT min_count_bfs(Machine const& machine,bool is_part2 = false) {
  INT min_count(-1);

  Expected end = to_expected(machine.lights);
  if (is_part2) {
    end = machine.joltage;
  }

  State state(end.size());
  std::queue<State> q;
  std::set<Expected> visited;

  q.push(state);
  visited.insert(state.value);

  size_t count{};
  while (!q.empty()) {

    State current = q.front();
    q.pop();

    if (count++ % 10000 == 0) {
      aoc::print(
         "\n{}: {}:{}",count,to_string(current.value),current.press_count);
      std::cout << std::flush;
    }

    if (current == end) {
      min_count = current.press_count;
      break; // BFS first = shortest button press path
    }

    // Explore options
    for (auto const& btn : machine.buttons) {
      State next = current;

      // Press button
      next.value = press(btn,next.value,is_part2);      
      next.press_count++;

      if (is_part2) {
        bool overflow{};
        for (int i=0;i<end.size();++i) {
          if (current.value[i] > end[i]) {
            overflow = true;
            break;
          }
        }
        if (overflow) continue;
      }

      if (!visited.contains(next.value)) {
        visited.insert(next.value);
        q.push(next);
      }
    }
  }

  return min_count;
}

std::optional<std::string> test_p1(int i,int test_ix,Machine const& machine) {
    if (test_ix > 0 and i==0) switch (test_ix) {
      // There are a few ways to correctly configure the first machine:
      // [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
      case 1: {
        // You could press the first three buttons once each, a total of 3 button presses.
        std::string expected = machine.lights;
        std::string lights(expected.size(),'.');
        lights = press(0,lights,machine);
        lights = press(1,lights,machine);
        lights = press(2,lights,machine);
        if (lights == expected) {
          return std::format("Test {} *PASSED*",test_ix);
        }
        else {
          return std::format(
             "Test {} failed. indicator lights '{}' differs from expected: '{}'"
            ,test_ix
            ,lights
            ,expected);
        }
      } break;
      case 2: {
        // You could press (1,3) once, (2,3) once, and (0,1) twice, a total of 4 button presses.
        std::string expected = machine.lights;
        std::string lights(expected.size(),'.');
        lights = press(1,lights,machine);
        lights = press(3,lights,machine);
        lights = press(5,lights,machine);
        lights = press(5,lights,machine);
        if (lights == expected) {
          return std::format("Test {} *PASSED*",test_ix);
        }
        else {
          return std::format(
             "Test {} failed. indicator lights '{}' differs from expected: '{}'"
            ,test_ix
            ,lights
            ,expected);
        }
      } break;
      case 3: {
        // You could press all of the buttons except (1,3) once each, a total of 5 button presses.
        std::string expected = machine.lights;
        std::string lights(expected.size(),'.');
        // lights = press(0,lights,machine);
        // lights = press(2,lights,machine);
        // lights = press(3,lights,machine);
        // lights = press(4,lights,machine);
        // lights = press(5,lights,machine);
        for (int bx=0;bx<machine.buttons.size();++bx) {
          if (bx != 1) lights = press(bx,lights,machine);
        }

        if (lights == expected) {
          return std::format("Test {} *PASSED*",test_ix);
        }
        else {
          return std::format(
             "Test {} failed. indicator lights '{}' differs from expected: '{}'"
            ,test_ix
            ,lights
            ,expected);
        }
      } break;
      case 4: {
        // However, the fewest button presses required is 2. One way to do this is by pressing the last two buttons ((0,2) and (0,1)) once each.
        INT min_count = min_count_bfs(machine);
        aoc::print("\nmin_count:{}",test_ix,min_count);
        if (min_count == 2) {
          return std::format("\ntest {} min_count:{} expected 2 *PASSED*",test_ix,min_count);
        }
        else {
          return std::format("\ntest {} min_count:{} NOT expected 2 *failed*",test_ix,min_count);
        }

      } break;
    }

    if (test_ix > 0 and i==1) switch (test_ix) {
      // The second machine can be configured with as few as 3 button presses:
      // [...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}
      case 5: {
        // One way to achieve this is by pressing the last three buttons ((0,4), (0,1,2), and (1,2,3,4)) once each.
        INT min_count = min_count_bfs(machine);
        aoc::print("\nmin_count:{}",test_ix,min_count);
        if (min_count == 3) {
          return std::format("\ntest {} min_count:{} expected 3 *PASSED*",test_ix,min_count);
        }
        else {
          return std::format("\ntest {} min_count:{} NOT expected 3 *failed*",test_ix,min_count);
        }
      } break;
    };

    if (test_ix > 0 and i==2) switch (test_ix) {
      // The third machine has a total of six indicator expected that need to be configured correctly:

      // [.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}
      case 6: {
        // The fewest presses required to correctly configure it is 2; one way to do this is by pressing buttons (0,3,4) and (0,1,2,4,5) once each.
        INT min_count = min_count_bfs(machine);
        aoc::print("\nmin_count:{}",test_ix,min_count);
        if (min_count == 2) {
          return std::format("\ntest {} min_count:{} expected 2 *PASSED*",test_ix,min_count);
        }
        else {
          return std::format("\ntest {} min_count:{} NOT expected 2 *failed*",test_ix,min_count);
        }
      } break;
    };

  return {};    
}

INT min_count_z3(Machine const& machine) {

  INT answer{};
  auto const& expected = machine.joltage;
  auto const& buttons = machine.buttons;

  // Use Z3 solver to find bs = min set of buttons, so that sum(bs) = expected
  // This will be the 'minmum button presses' to get the expected joltage.

  z3::context ctx;
  z3::optimize optimizer(ctx);

  // Create integer variables for each button (how many times each button is pressed)
  std::vector<z3::expr> button_counts;
  for (size_t i = 0; i < buttons.size(); ++i) {
    button_counts.push_back(ctx.int_const(("b" + std::to_string(i)).c_str()));
    // Each button must be pressed >= 0 times
    optimizer.add(button_counts[i] >= 0);
  }

  // For each joltage position, the sum of button presses that affect it must equal the expected value
  for (size_t pos = 0; pos < expected.size(); ++pos) {
    z3::expr sum = ctx.int_val(0);
    for (size_t btn_idx = 0; btn_idx < buttons.size(); ++btn_idx) {
      // Check if this button affects this position
      if (std::find(buttons[btn_idx].begin(), buttons[btn_idx].end(), pos) != buttons[btn_idx].end()) {
        sum = sum + button_counts[btn_idx];
      }
    }
    optimizer.add(sum == (int)expected[pos]);
  }

  // Minimize the total number of button presses
  z3::expr total = ctx.int_val(0);
  for (auto const& bc : button_counts) {
    total = total + bc;
  }
  optimizer.minimize(total);

  // Solve
  if (optimizer.check() == z3::sat) {
    z3::model m = optimizer.get_model();
    answer = m.eval(total).get_numeral_int64();
  }

  return answer;
}

std::optional<INT> min_count_ilp(
   std::vector<unsigned> const& row_ixs
  ,unsigned rix
  ,std::set<unsigned> bound
  ,std::set<unsigned> unbound
  ,std::vector<unsigned> candidates
  ,std::vector<std::vector<unsigned>> const& Ab) {

  const unsigned C = Ab[0].size();
  const unsigned R = Ab.size();

  if (rix == R) {
    // return found candidate
    return std::accumulate(candidates.begin(),candidates.end(),INT{0});
  }

  auto r = row_ixs[rix];
  std::vector<unsigned> unkown_ixs{};
  for (int c=0;c<C-1;++c) {
    if (Ab[r][c] != 0) {
      if (unbound.contains(c)) unkown_ixs.push_back(c);
    }
  }

  const unsigned U = unkown_ixs.size();
  unsigned rhs = Ab[r][C-1];

  aoc::print("\n    r:{} c: {} b:{} u:{}",r,candidates,bound,unkown_ixs);

  unsigned bound_sum{};
  for (auto i : bound) bound_sum += candidates[i];
  aoc::print(" bs:{}",bound_sum);
  // xi = rhs - bound_sum - xj;
  if (bound_sum > rhs) return std::nullopt; // Infeasable

  const auto RHS = rhs - bound_sum;

  switch(U) {
    case 0: {
      // No unknowns
      if (RHS == 0 and rix < R) {
        return min_count_ilp(
           row_ixs
          ,++rix
          ,bound
          ,unbound
          ,candidates
          ,Ab
        );
      }
      return std::nullopt; // infeasable
    } break;
    case 1: {
      // Bind to rhs
      auto uix = unkown_ixs[0];
      if (unbound.contains(uix)) {
        candidates[uix] = RHS;
        unbound.erase(uix);
        bound.insert(uix);
        if (rix<R) {
          return min_count_ilp(
            row_ixs
            ,++rix
            ,bound
            ,unbound
            ,candidates
            ,Ab
          );
        }
      }
    } break;
    case 2: {
      // xi + xj + sum(bound)  = rhs;
      auto i = unkown_ixs[0];
      auto j = unkown_ixs[1];
      if (rix < R and unbound.contains(i) and unbound.contains(j)) {
        unbound.erase(i);
        bound.insert(i);
        unbound.erase(j);
        bound.insert(j);
        unsigned candidate = std::numeric_limits<unsigned>::max();
        ++rix;
        const auto RHS = rhs - bound_sum;
        for (unsigned xj=0;xj<=RHS;++xj) {
          auto xi = RHS -xj;
          candidates[i] = xi;
          candidates[j] = xj;
          aoc::print(" xi:{},xj:{}",xi,xj);
          auto result = min_count_ilp(
            row_ixs
            ,rix
            ,bound
            ,unbound
            ,candidates
            ,Ab
          );
          if (result and result.value() < candidate) candidate = result.value();
        } // for xi,xj
        aoc::print("\n    -> min:{}",candidate);
      } // if valid
    } break;
  }
  // Debug - return candidate sum (Remove when fully implemented above)
  return std::accumulate(candidates.begin(),candidates.end(),INT{0});

  // Final - return infeasable
  return std::nullopt;
}

std::optional<INT> min_count_ilp(Machine const& machine) {

  auto const& buttons = machine.buttons;
  auto const& joltage = machine.joltage;
  const unsigned C = buttons.size()+1; // Ab columns = button count plus rhs joltage
  const unsigned R = joltage.size(); // Ab rows = joltage components count
  std::vector<std::vector<unsigned>> Ab(R,std::vector<unsigned>(C));
  for (unsigned c=0;c<C-1;++c) {
    auto const& button = buttons[c]; 
    for (auto jx : button) {
      Ab[R-jx-1][c] = 1; 
    }
  }
  for (auto r=0;r<R;++r) {
    Ab[r][C-1] = joltage[R-r-1];
  }
  aoc::print("\nbuttons:{} joltage:{}",buttons,joltage);
  aoc::print("\nAb: {}x{}{}",C,R,Ab);
  using MinMax = std::pair<unsigned,unsigned>;
  std::vector<unsigned> col_nonz_count(C);
  std::vector<unsigned> row_nonz_count(R);
  for (unsigned r=0;r<R;++r) {
    for (unsigned c=0;c<C-1;++c) {
      if (Ab[r][c] != 0) {
        ++col_nonz_count[c];
        ++row_nonz_count[r];
      }
    }
  }
  MinMax col_nonz_min_max{
     *std::ranges::min_element(col_nonz_count)
    ,*std::ranges::max_element(col_nonz_count)
  };
  MinMax row_nonz_min_max{
     *std::ranges::min_element(row_nonz_count)
    ,*std::ranges::max_element(row_nonz_count)
  };

  aoc::print("\ncol_nonz_count:[{}..{}] {}",col_nonz_min_max.first,col_nonz_min_max.second,col_nonz_count);
  aoc::print("\nrow_nonz_count:[{}..{}]] {}",row_nonz_min_max.first,row_nonz_min_max.second,row_nonz_count);

  std::vector<MinMax> x_constraints(C-1,{0,std::numeric_limits<unsigned>::max()});
  for (unsigned c=0;c<C-1;++c) {
    for (unsigned r=0;r<R;++r) {  
      // Bound unknown xc to <= min joltage component.
      if (Ab[r][c] != 0) {
        // button c can't be pressed more than any joltage component it affects.
        x_constraints[c].second = std::min(x_constraints[c].second,Ab[r][C-1]);
      }
    }
  }
  aoc::print("\nx_constraints:{}",x_constraints);

  // Sort rows in order of unknowns
  std::vector<unsigned> row_ixs(R);
  for (unsigned r=0;r<R;++r) row_ixs[r] = r;
  std::ranges::sort(row_ixs,[&row_nonz_count](auto lhs,auto rhs){
    return row_nonz_count[lhs] < row_nonz_count[rhs];
  });
  aoc::print("\nrow_ixs:{}",row_ixs);

  std::vector<unsigned> candidates(C-1);
  std::set<unsigned> unbound{};
  for (unsigned c=0;c<C-1;++c) {
    candidates[c] = x_constraints[c].second;
    unbound.insert(c);
  }
  std::set<unsigned> bound{};
  auto candidate = min_count_ilp(
     row_ixs
    ,0
    ,bound
    ,unbound
    ,candidates
    ,Ab
  );

  return candidate;
}


std::optional<std::string> test_p2(int i,int test_ix,Machine const& machine) {
  if (i==0) switch(test_ix) {

    case 1: {
      // If the current joltage levels were {0,1,2,3}, pushing the button (1,3) would change them to be {0,2,2,4}.
      Joltage expected{0,2,2,4};
      Joltage joltage{0,1,2,3};
      auto next = press(machine.buttons[1],joltage,true);
      if (next == expected) {
        return std::format(
          "Test {} next:{} is expected {} *PASSED*"
          ,test_ix
          ,to_string(next)
          ,to_string(expected));
      }
      else {
        return std::format(
          "Test {} next:{} is NOT expected {} *failed*"
          ,test_ix
          ,to_string(next)
          ,to_string(expected));
      }
    } break;
    case 2: { 
      // Configuring the first machine's counters requires a minimum of 10 button presses. 
      // One way to do this is by pressing (3) once, (1,3) three times, 
      // (2,3) three times, (0,2) once, and (0,1) twice.
      INT min_count = min_count_z3(machine);
      aoc::print("\ntest {} min_count:{}",test_ix,min_count);
      if (min_count == 10) {
        return std::format("\ntest {} min_count:{} expected 10 *PASSED*",test_ix,min_count);
      }
      else {
        return std::format("\ntest {} min_count:{} NOT expected 10 *failed*",test_ix,min_count);
      }
    } break;
    case 3: {
      auto maybe_min_count = min_count_ilp(machine);
      aoc::print("\ntest {} min_count:{}",test_ix,maybe_min_count);
      if (maybe_min_count.value_or(-1) == 10) {
        return std::format("\ntest {} min_count:{} expected 10 *PASSED*",test_ix,maybe_min_count);
      }
      else {
        return std::format("\ntest {} min_count:{} NOT expected 10 *failed*",test_ix,maybe_min_count);
      }      
    } break;
  }

  if (i==1) switch(test_ix) {

    case 3: {
      // Configuring the second machine's counters requires a minimum of 12 button presses. 
      // One way to do this is by pressing (0,2,3,4) twice, (2,3) five times, and (0,1,2) five times.
      INT min_count = min_count_z3(machine);
      aoc::print("\nmin_count:{}",test_ix,min_count);
      if (min_count == 12) {
        return std::format("\ntest {} min_count:{} expected 12 *PASSED*",test_ix,min_count);
      }
      else {
        return std::format("\ntest {} min_count:{} NOT expected 12 *failed*",test_ix,min_count);
      }
    } break;
  }

  if (i==2) switch(test_ix) {

    case 4: {
      // Configuring the third machine's counters requires a minimum of 11 button presses. 
      // One way to do this is by pressing (0,1,2,3,4) five times, (0,1,2,4,5) five times, and (1,2) once.    
      INT min_count = min_count_z3(machine);
      aoc::print("\nmin_count:{}",test_ix,min_count);
      if (min_count == 11) {
        return std::format("\ntest {} min_count:{} expected 11 *PASSED*",test_ix,min_count);
      }
      else {
        return std::format("\ntest {} min_count:{} NOT expected 11 *failed*",test_ix,min_count);
      }
    } break;
  }
  
  return {};
}

std::optional<std::string> solve(PuzzleArgs puzzle_args,bool for_part2 = false) {

  auto test_ix = puzzle_args.meta().m_maybe_test.value_or(0);

  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse(in);

  // Solve here
  UINT candidate{};

  for (size_t i=0;i < model.size();++i) {
    auto const& machine = model[i];
    
    aoc::print("\nmachine:{} joltage:{}",i,to_string(machine.joltage));

    if (!for_part2) {
      if (auto test_result = test_p1(i,test_ix,machine)) {
        return *test_result;
      }

      candidate += min_count_bfs(machine,for_part2);

    }
    else {
      if (auto test_result = test_p2(i,test_ix,machine)) {
        return *test_result;
      }

      auto z3_result = min_count_z3(machine);
      candidate += z3_result;
      if (auto ilp_result = min_count_ilp(machine)) {
        aoc::print("\n\nilp_result:{}",ilp_result);
        if (ilp_result.value() != z3_result) {
          aoc::print(" DIFFERS from z3_result:{}",z3_result);
        }
        else {
          aoc::print(" **SAME** as z3_result:{}",z3_result);
        }
      }

    }

  }

  if (!for_part2) {
    if (test_ix == 7) {
      // So, the fewest button presses required to correctly configure the indicator expected on all of the machines is 2 + 3 + 2 = 7.
      if (candidate == 7) {
        return std::format("Test {} candidate:{} *PASSED*",test_ix,candidate);
      }
    }
  }
  else {
    if (test_ix == 7) {
      // So, the fewest button presses required to correctly configure the joltage level counters 
      // on all of the machines is 10 + 12 + 11 = 33.    
      if (candidate == 33) {
        return std::format("Test {} candidate:{} *PASSED*",test_ix,candidate);
      }
    }
  }



  // return {};
  // return std::format("Not yet fully implemented");
  return std::format("{}",candidate);

} // solve


std::optional<std::string> p1(PuzzleArgs puzzle_args) {
  return solve(puzzle_args); // 520
} // p1

std::optional<std::string> p2(PuzzleArgs puzzle_args) {
  return solve(puzzle_args,true);
} // p2

std::optional<std::string> day(PuzzleArgs puzzle_args) {
  aoc::print(
    "\nday10 part:{} debug:{}"
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
