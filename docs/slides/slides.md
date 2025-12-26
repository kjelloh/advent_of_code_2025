---
marp : true
---
# day 10 part 2

What is the minimum number of button presses to produce a defined 'joltage'?

Example:

- Target Joltage {3,5,4,7}
- Buttons (1,3) (2) (2,3) (0,2) (0,1)
- Initial joltage counters {0,0,0,0}

What kind of problem is this?

<!-- 

- The 'joltage' has components 0..N-1, like the example {3,5,4,7}
- A 'button' defines the joltage counter indicies it increments, like (1,3).
  When pushed it increments joltage counter 1 and 3 by one.

Pause the video here to think about this. How would you solve it?

-->

---

We can model this problem as a task of finding a vector sum.

```text
x0 * v(1,3) + x1 * v(2)  + x2 * v(2,3) + x3 * v(0,2) + x4 * v(0,1) = {3,5,4,7}
```

- x0 is the number of times we press button (1,3)
- Button (1,3) increments counter 1 and 3
                   0 1 2 3
- v(1,3) = vector (0,1,0,1)

What is the minimal sum(x1) that satisfies the equation?

How can we solve this?

<!-- 

- v of indecies is the vector representing the vector 'value'
- v(1,3) is the vector (0,1,0,1)
  This will model that the button adds 1 to counter 1 and 3

-->

---

This is an Integer Linear Programming problem.

- Find integer solutions to a system of integer equations

This type of problem is NP hard!

- VAST search space
- But: Easy to check solution

<!--
- VAST search space for candidate solutions x0,x1,..xi..
- Easy to check matrix A times vector x = joltage vector b
-->

---

I found two ways to solve day10.

First:

- Model as Ax == b

Then two options: 

1. Feed to z3 solver
2. Feed to hand rolled solver

<!--

This video is about my hand rolled solver.

- All solutions I have found on the web uses the z3 solver
- But surely there should be a feasable hand-rolled solution also for day10 part 2?
- And YES - I implemented one that worked on my input

-->

---

How can we:

1. Model the problem as Ax=b?
2. Search the space of possible x to find min(sum(xi))?

Pause the video here to think about this.

<!--

- To model our problem as matrix A times vector X equals vector b
  IS a known problem.
- Searching all x that fullfills this equation system
  has no general solution
- An integer solution to integer equation system Ax=b 
  IS NP hard
- But we expect advent of code problem to have features we can exploit!

-->

---

Model the problem as system Ax=b

```text
- Buttons: (3) (1,3) (2) (2,3) (0,2) (0,1)
- Joltage: {3,5,4,7}
```

Becomes:

```c++
i:   0 1 2 3
x0* (0 0 0 1) // (3) 
x1* (0 1 0 1) // (1,3)
x2* (0 0 1 0) // (2)
x3* (0 0 1 1) // (2,3)
x4* (1 0 1 0) // (0,2)
x5* (1 1 0 0) // (0,1)
+   ------
    (3 5 4 7)
```
---

```c++
i:   0 1 2 3
x0* (0 0 0 1) // (3) 
x1* (0 1 0 1) // (1,3)
x2* (0 0 1 0) // (2)
x3* (0 0 1 1) // (2,3)
x4* (1 0 1 0) // (0,2)
x5* (1 1 0 0) // (0,1)
+   ------
    (3 5 4 7)
```
Becomes:
```text
1 1 0 1 0 0 x0  = 7     // x0 + x1 + a3 = 7
0 0 1 1 1 0 x1    4     // x2 + x3 + a4 = 4
0 1 0 0 0 1 x2    5     // x1 + x5      = 5
0 0 0 0 1 1 x3    3     // x4 + x5      = 3
            x4
            x5
```

---

Is there feature we can exploit?

```text
1 1 0 1 0 0 x0  = 7     // x0 + x1 + a3 = 7
0 0 1 1 1 0 x1    4     // x2 + x3 + a4 = 4
0 1 0 0 0 1 x2    5     // x1 + x5      = 5
0 0 0 0 1 1 x3    3     // x4 + x5      = 3
            x4
            x5
```

Pause the video here to think about this.

<!--

Are there features showing in the example we can exploit?

- YES - there is.
- The equation system is of a nature that
  dramatically reduces the space of possible x:s to try!
- Look at the the two bottom equations.

-->

---

The equation:  
```sh
x4 + x5      = 3
```

Means:

```sh
x4 = 3 - x5 -> (x4,x5) = (0,3),(1,2),(2,1),(3,0),
```

<!--

- x4 + x5 equals 3
- Means x4 is 3 minus x5
- Giving us a set of possible pairwise x4,x5
- This is only 4 possible combinations we have to try! 

-->

---

Solver in C++.

1. Creating the Ab matrix

First:

```c++
const unsigned C = buttons.size()+1; // Ab columns = button count plus rhs joltage
const unsigned R = joltage.size(); // Ab rows = joltage components count
```

Then:

```c++
  std::vector<std::vector<unsigned>> Ab(R,std::vector<unsigned>(C));
```

<!--
We create the Ab matrix as if to solve using gaussian elimination

- Ab is to have columns with the button vectors
- Ab is to have the rightmost column as the joltage
- So the size is R rows (one for each joltage component)
- And C columns (number of buttons plus the joltage vector column)
-->

---

Fill in the button vector columns

```c++
  for (unsigned c=0;c<C-1;++c) {
    auto const& button = buttons[c]; 
    for (auto jx : button) {
      Ab[R-jx-1][c] = 1; 
    }
  }
```

Fill in the joltage vector column

```c++
  for (auto r=0;r<R;++r) {
    Ab[r][C-1] = joltage[R-r-1];
  }
```

<!--
- An Ab column for button c has a 1 for each joltage counter it incements
- Note the loop for column c = 0..C-2 for buttons
-->
---

Rig the solver to apply equations in the order of unknowns

```c++
  std::vector<unsigned> row_ixs(R);
  for (unsigned r=0;r<R;++r) row_ixs[r] = r;
  std::ranges::sort(row_ixs,[&row_nonz_count](auto lhs,auto rhs){
    return row_nonz_count[lhs] < row_nonz_count[rhs];
  });
```

<!--
We have created a row_nonz_count

- I has the unknowns for each row equation in Ab
- We use it to create a vector with the order we shall apply the rows
- We sort it to apply equations with the least unknowns first
-->
---

The recursive solver

```c++
std::optional<INT> min_count_ilp(
   std::vector<unsigned> const& row_ixs // Ordered row indicies
  ,unsigned rix                         // Current row index
  ,std::set<unsigned> bound             // bound x indicies
  ,std::set<unsigned> unbound           // not yet bound x indicies
  ,std::vector<unsigned> candidates     // candidate xi values
  ,std::vector<std::vector<unsigned>> const& Ab);
```

<!--
I came up with a recursive solver API
- row_ixs is the ordered row indicies
- rix is the current index 0..R-1 into row_ixs
- bound is the set of x indices with bound values in candidates
- And unbound is the opposite x indicies
- candidates is the xi values we have so far
- And Ab is the full system matrix we use to constraint our search space
-->

---

The min_count_ilp solver.

Count the free unknowns in current row equation to apply.

```c++
  auto r = row_ixs[rix];
  std::vector<unsigned> unkown_ixs{};
  for (int c=0;c<C-1;++c) {
    if (Ab[r][c] != 0) {
      if (unbound.contains(c)) unkown_ixs.push_back(c);
    }
  }
```

<!--
- Current row is given by current row index (look up in ordered row_ixs)
- Count if contributes (non zero coefficient) and not yet bound
-->
---

The min_count_ilp solver.

Branch on number of unbound unknowns.

```c++
  const unsigned U = unkown_ixs.size();
  ...
  switch(U) {
    case 0: // No unknowns
    case 1: // One unknown
    case 2: // two unknowns
  }
```

<!--
It turns out we only need to handle 0,1 or 2 unknowns
- I have not checked if all gets 'hit' by day 10 input?
- I DO know that the '2 unknowns' is the key to make the solver work
-->

---

The min_count_ilp solver.

```c++
    case 2: {   // xi + xj + sum(bound)  = rhs;
      auto i = unkown_ixs[0];
      auto j = unkown_ixs[1];
      unbound.erase(i); bound.insert(i);
      unbound.erase(j); bound.insert(j);
      unsigned candidate = std::numeric_limits<unsigned>::max();
      ++rix;
      const auto RHS = rhs - bound_sum;
      for (unsigned xj=0;xj<=RHS;++xj) {
        auto xi = RHS -xj;
        ...
      } // for xi,xj
      return candidate;
    } break;
```

<!--
When we have two unbound unknowns we can:
- Bound the two into pairs based on the equation
- In the general case the equation becomes xi + xj + sum(bound) = right hand side
- Thus we can generate bound pairs xi,xj based on the bound sum and rhs
-->

---


Two unknowns - Recurse on two unknowns bound (as is possible by applied equation)

```c++
  // xi + xj + sum(bound)  = rhs;
  ...
  for (unsigned xj=0;xj<=RHS;++xj) {
    auto xi = RHS -xj;
    candidates[i] = xi; candidates[j] = xj;
    auto result = min_count_ilp(
      row_ixs
      ,rix
      ,bound
      ,unbound
      ,candidates
      ,Ab);
    ...
  } // for xi,xj
```

<!--
We can now try out each possible way to pair the two unknowns
- We bind the possible pair
- Then we recurse on this state
- The result is either INF (none) or a soluton
-->

---

Two unknowns - Aggregate the current min candidate

```c++
  ...
  if (result and result.value() < candidate) 
    candidate = result.value();
```

<!--
We aggregate each result to an overall min candidate result
-->

---

Two unknowns - pseudo code

```c++
  // xi + xj + sum(bound)  = rhs;
  // RHS = rhs - sum(bound) > 0

  i,j = unbound_unknowns(row,bound)

  init(best)

  bind(i),bind(j)
  for [xi,xj] in cartesian(0..RHS,RHS..0)

    candidates[i] = xi; candidates[j] = xj;

    auto result = min_count_ilp(next_row,candidates,bound,...)

    best = min(best,result)
  } // for xi,xj
  return candidate;

```

<!--
It seems we can boil down the 'two unkown' case to:
- Given two free unknowns, and RHS = rhs - sum(bound) > 0
- identify i,j as the unbound unknowns
- We initialise a 'best' as the base to aggregate a best candidate
- We loop over all possible xi,xj
- We recurse down for each possible pair
- And aggregate the best of the return values
-->

---

The min_count_ilp solver.

- One free unknown

```c++
    // xi + sum(bound)  = rhs;
    auto uix = unkown_ixs[0];
    candidates[uix] = RHS;
    unbound.erase(uix); bound.insert(uix);
    if (rix<R) {
      return min_count_ilp(
        row_ixs
        ,++rix
        ,bound
        ,unbound
        ,candidates
        ,Ab
      );

```

<!--
For the case of only one free unknown
- We can bind the unknown to RHS
- And recurse down to next row
-->

---

Recursion end condition (valid solution)

```c++
  if (rix == R) {
    for (unsigned r=0;r<R;++r) {
      auto rhs = Ab[r][C-1];
      unsigned lhs{};
      for (unsigned c=0;c<C-1;++c) {
        lhs += Ab[r][c] * candidates[c];
      }
      if (lhs != rhs) return std::nullopt; // not a valid candidate
    }
    // candidates fullfills all equations
    return std::accumulate(candidates.begin(),candidates.end(),INT{});
  }
```

<!--
When we have applied all row equations
- Fingers crossed we have a valid solution
- If we do, return it (the xi sum)
-->

---

Pruning the Infeasable

```c++
  // sum(unbound xi:s) = rhs - bound_sum
  if (bound_sum > rhs) {
    aoc ::print(" > rhs:{} INF",rhs);
    return std::nullopt; // Infeasable
  }
```

<!--
We can detect infeasable solution
- In general sum(free xi:s) + sum(bound xj:s) = rhs
- And all xi and rhs are > 0
- Means rhs - sum(bound xj:s) must be >= 0
- If it it not, no valid solution is feasable
-->

---

Improvements to try:

- We do not need both a bound and unbound set
- I suspect the 'valid solution' check is not needed?
  We should not generate any invalid candidates?
- We should be able to apply unbound candidates as upper bounds?
  - My code initiates them to an upper bound
  - So maybe we can prune even harder based on free unknowns
    current upper bound values?
- The code is not cleaned up
  It contains older diagnostic code and experiments
  that can be removed.
- I am cursious what our AI friend could suggest
  based on the code now works (safe to have AI propose refactorings for efficiency?)

---

# day7 part 2

How many paths can a single particle take to reach the bottom row?

```text
.......S.......             1
.......|.......             1
......|^.......           1   1
......|........           1   1
.....|^.^......         1   2   1
.....|.........         1   2   1
```

<!-- 
Each time it hits a splitter on its path it can chose left or right (if there is room).

So at each row we can keep a running count of the number of ways the particle can reach a column on that row.
-->

---

Can we count the number of possible paths by using the registered beams?

```text
.......S.......
.......|.......          4
......|^|......         2 2
......|.|......         2 2
.....|^|^|.....       1  1  1
.....|.|.|.....       1  1  1
```

```sh
for row in num_rows down to 1
  for beam in beams on row
    if split_from_left_above
      increment above left_beam path options counter
    if split_from_right_above
      increment above right_beam path options counter
```

The options counter for the initial beam on the top row is now the sum of all possible paths?

---
# Advent of Code - Day 5: Cafeteria (part 1)

- [https://adventofcode.com/2025/day/5](https://adventofcode.com/2025/day/5)

We have a list of ranges of IDs referring to 'fresh' ingridients. 

And we have a list of ingredient IDs.

How can we find all ingredients that are listed as 'fresh'?

---

Let's examine a list of 'fresh' ingredient ID-ranges:

```text
3-5
10-14
```

Is ingredient with ID 3 freash?
Is ingredient with ID 7 fresh?

---

Assume we have the following list of ranges of IDs that refers to fresh ingredient IDs. And an accompanying list of IDs referring to the ingredients we have.

```text
**fresh**             **ingredient**
3-5                   1
10-14                 5
16-20                 8
12-18                 11
                      17
                      32
```

How may of our ingredients are 'fresh'?

<!-- 

- Ingredient ID 1 is spoiled because it does not fall into any range.
- Ingredient ID 5 is fresh because it falls into range 3-5.
- Ingredient ID 8 is spoiled.
- Ingredient ID 11 is fresh because it falls into range 10-14.
- Ingredient ID 17 is fresh because it falls into range 16-20 as well as range 12-18.
- Ingredient ID 32 is spoiled.

So, in this example, 3 of the available ingredient IDs are fresh.
-->

---

```text
**fresh**             **ingredient**
3-5                   1
10-14                 5
16-20                 8
12-18                 11
                      17
                      32

- Ingredient ID 1 is spoiled because it does not fall into any range.
- Ingredient ID 5 is fresh because it falls into range 3-5.
- Ingredient ID 8 is spoiled.
- Ingredient ID 11 is fresh because it falls into range 10-14.
- Ingredient ID 17 is fresh because it falls into range 16-20 as well as range 12-18.
- Ingredient ID 32 is spoiled.
```

How can we implement this in code?

---

```text
**fresh**             **ingredient**
3-5                   1
10-14                 5
16-20                 8
12-18                 11
                      17
                      32
```

```sh
for id in ingredients
  if (any_of(fresh_ranges) contains id) then increment count
```

<!-- 
To count the number of fresh ingredients we can do as follows. For each ingredient id, if any of the ranges degining fresh ingredient IDs contains this id, then increment the fresh ingredient counter.
-->

---

```c++
// C++
  auto model = parse(in);
  size_t acc{};
  for (auto id : model.second) {
    auto is_fresh = std::ranges::any_of(model.first,[id,&log](auto range){
      return (range.first <= id) and (id <= range.second);
    });
    if (is_fresh) ++acc;
  }
```

<!-- 
We parse the input into a model with 'first' being all the ranges and 'second' being our ingredient id:s. We then iterates over our ingredient id:s, and for each we call 'any_of on our ranges with a lambda predicate that checks the examioned id against each range we have. Then, if a match was found, we increment the accumulator to count the examined id as fresh.
-->

---
# Advent of Code - Day 5: Cafeteria (part 2)

- [https://adventofcode.com/2025/day/5](https://adventofcode.com/2025/day/5)

We now focus on the list of ranges of IDs referring to 'fresh' ingridients. 

How can we count all ingredient ID:s that are listed as 'fresh'?

---

Let's examine a list of 'fresh' ingredient ID-ranges:

```text
3-5
16-20
12-18
```

Given these three ranges - how many ingredient IDs refers to a fresh ingredient?

---
**fresh**
3-5
16-20
12-18

```text

  3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
  |   |                |            |    |     |
  -----                |           ------|------
                       |                 |
                       -------------------
```
**Overlapping ranges**

What algorithm can we use to solve this problem?

---

We can use 'Interval Merging'

- Sort intervals by their start points
- Merge overlapping intervals into disjoint intervals
- Sum the lengths of the merged intervals

How can we implement this in code?

---

Given we have sorted all ranges on their start position we can merge them into disjunct ranges.

```c++
// C++ 'merge ranges'
std::vector<std::pair<size_t,size_t>> disjunct{};
for (auto const& r : ranges) {
  if (
        (disjunct.size() == 0)
    or (r.first > disjunct.back().second + 1)) disjunct.push_back(r);
  else {
    // Overlaps or adjacent -> extend current
    disjunct.back().second = std::max(r.second,disjunct.back().second);
  }
}
```

<!-- 
We create the disjunct container and then we loop through the sorted ranges. If the disjunct comtainer is empty, or - the examined range startes after the last disjunct one, then we add it as a disjunct one.

Otherwise, we adjust the last disjunct range to accomodate the range we are examining. We do this by extending the disjunct range if the examined range covers a larger range.
-->

---

We *really* need a 'range merging' algorithm to handle **very large** ranges of the input!

E.g.,

```text
413219263784584-421133392130670
115402151906587-115766841688429
111263823867195-112047960488389
...
```

