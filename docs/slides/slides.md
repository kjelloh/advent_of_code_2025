---
marp : true
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

