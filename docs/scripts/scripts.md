# day9 part 2

We have a list of positions of red tiles on a 2D 'floor'. 

Implicitally we also have green tiles between any neighbouring red tiles on the same row and also between neighbouring red tiles in the same column.

Some (all?) red tiles with connecting green tiles makes up an enclosed area of tiles. Any such enclosure is also filled wih green tiles.

Our task is now to pick two red tiles on this enclosure such that when used as corrners of a rectangle, this rectangle encloses only red or green tiles. And also such that we get the largest possible area of a rectangle with these properties.

It seems we are to find the largest rectangle totally within the enclosure of the convex hull made up of red corner tiles?

- A corner tile is one that can be connected to another red tile on the same row or in the same column.
- It seems a corner tile with its edges to its connected corners models an 'L'?
- Each corner 'L' can be one of four that differs in 90 degrees of rotation?
- Do we care about corner 'L' rotations?

One can queston - can we create a convex hull that uses all the red tiles?

- We cant use red tiles that does not connect to two other red tiles.
 
 Wait! Some red tiles connect to its neighbours in a straight line (does NOT create a corner)!

 - At this stage I do not dare to propose we can ignore staright-line-red-tile-combos?
 - We may need intermediate red tiles to anchor a rectangle?

So what operations do we need to implement?

And what known algorithms can we use to solve this problem?

Also, have I understood enough of the intricasies of this problem to solve it?

Wait! The red tiles may make up more than one enclosure of red and green tiles?

- It seems we need a way to first identify all contigous enclosed areas?
- Possibly eliminate smaller encolures that are totally within a larger enclosure?
  (safe(?) and may be required to shrink search space for largest rectangle?)
- Then we need to pick two red tiles on each circumfenced area for a rectangle candidate.
- For each rectangle candidate we need to skip any rectangle that encloses non red or green tiles.
- For each valid rectangle candidate we need to record its area.
- When we have gone through all rectangle candidates we are to pich the candodate with the largest area.

I wonder - can I pick red tiles to connect plus pick what neighbours to connect to and end up with different candidate hulls'?

From part two problem statement we read:

```text
In your list, every red tile is connected to the red tile before and after it by a straight line of green tiles. The list wraps, so the first red tile is also connected to the last red tile. Tiles that are adjacent in your list will always be on either the same row or the same column.
```

- It seems each entry in the list defines a tile that share row or column with its adjacent tile entry?
- Does this mean the list in fact strings the red tiles together into a hull?
- YES - It sais '..the first red tile is also connected to the last red tile'.

Can we do 'coordinate compression'?

- It seems we can use 'coordinate compression' based on all edges being in one axies direcion only.
- So we can map each adjacent pairs of tiles in the list to a range in x and y.
- We now know Each such range is 'INSIDE' meaning consists of red start and end tiles 
  and connecting green tiles.
- It seems we can sort the x-ranges and y-ranges separatly?
  They form 'bands' on the grid and we can still look for the overlapping bands
  to get the true 'INSIDE' tiles.

Let's examine our example:

```text
7,1
11,1
11,7
9,7
9,5
2,5
2,3
7,3
```

- In x (col) we have: 7,11,11,9,9,2,2,7 or sorted x: 2,2,7,7,9,9,11,11
- In y (row) we have 1,1,7,7,5,5,3,3 or sorted y: 1,1,3,3,5,5,7,7

```text
yx- 0         1
|   0123456789012
00  .."...."."."..
 1  =="====#="=#==
 2  .."...."."."..
 3  ==#====#="="==
 4  .."...."."."..
 5  ==#===="=#="==
 6  .."...."."."..
 7  =="===="=#=#==
 8  .."...."."."..
```

The idea seems to be that we scan these ranges separatly and toggles OUTSIDE,INSIDE,OUTSIDE... as we encounter them. That is, we implicit the ranges between the initial INSIDE ranges and mark them as OUTSIDE.

- A grid position x,y is now INSIDE of x is in an INSIDE x-range and y in INSIDE y-range.
- That is, an overlap of INSIDE x-band and INSIDE y-band defines an INSIDE grid position.

Our example is a bit obscure though in that they define overlapping ranges. Because our problem defines the border tiles to be included in the covered area.

Let's study the x-range 'bands' defined by the 'borders' 2,2,7,7,9,9,11,11.

```text
    0         1
    0123456789012
    ---
      ------
           ---
             ---
               ----...
    001111111111000..
```

It seems we need a 'compressed xy-matrix' with postitions representing our input inside ranges and intermediate outside ranges?

- So we can transform inside-x borders 2,2,7,7,9,9,11,11 
  to pairwise (-,2),(2,2),(2,7),(7,7),(7,9),(9,9),(9,11),(11,11),(11,+)
  Compressed:   0     1     2     3     4     5     6       7       8

Wait! Lets condense our knowledge.

- We have a list of positions of red tiles on a large sparse floor (2D grid) of tiles.
- Positions are given as x,y (column,row).
- Each tile in our list 'connects' to its adjacent tile in the list with implied green tiles.
- The listed tiles defies a closed loop (last and first tile also connects)
- The red tiles are arranged so that green tiles span only rows or columns (no diagonals)
- All tiles inside the loop of red and green tiles are also implicitally green.

We are to pick two red tiles as corners of a rectangle that is as large as we can make it given some constraints.

- The implied rectange, including its border, must only contain red or green tiles.

Looking at the input data we can observe the following:

- The count of red tiles range from around 10 (example) to around 500 (todays problem).
- The floor grid positions are within 15x15 for the example. 
- But the floor grid is in the range 100 000 x 100 000 in todays problem.

So we need a way to 'compress' this problem.

We can do this by 'striping' the floor grid based on ranges of tiles in x and y.

- We 'stripe' by creating open ended ranges from the ones we know (connected red tiles)
  with the gaps between also inserted.
- We then make the stripes in each dimension unique.
  (No x stripes overlap other x-stripes and no y-stripes overlap other y-stripes)
- Now we have 'striped' the grid into 'blocks' instead of single tiles.
- And the overlapping x and y 'on-stripes' define 'on-blocks'.

The rectange we are looking for will now also be compressed into blocks in the compressed domain.

- 'Stripes' in the grid-domain maps to indexes in the 'stripe domain'.
- A rectangel with red tiles corners on the grid maps to a rectangle with index-coordinates
  in the compressed domain.

How can we write this algorithm?

Lets make the example big with the red tiles (2,5),(4000,5),(4000,7000),(2,7000). Now how would the compressed 'stripe domain' look like?

Make X stripes:

- exact stripe for x = 2 → range [2], width = 1
- gap stripe for 3..3999 → range (3..3999), width = 3997
- exact stripe for x = 4000 → range [4000], width = 1

So X stripes (compressed indices):

- cx = 0 → real x = [2], width 1
- cx = 1 → real x = [3..3999], width 3997
- cx = 2 → real x = [4000], width 1

make Y stripes:

- exact stripe for y = 5 → height = 1
- gap stripe for 6..6999 → height = 6994
- exact stripe for y = 7000 → height = 1

So Y stripes (compressed indices):

- cy = 0 → real y = [5], height 1
- cy = 1 → real y = [6..6999], height 6994
- cy = 2 → real y = [7000], height 1

Compressed grid size: 3 × 3 blocks.

Seems promising!

# day8 part 1

So todays problem seems ro be about connected graphs?

We have 'junction boxes' in 3D space given by XYZ in a list like:

```text
in[   0][0.. 10]: '162,817,812' 
in[   1][0..  8]: '57,618,57' 
in[   2][0.. 10]: '906,360,560' 
in[   3][0.. 10]: '592,479,940' 
in[   4][0.. 10]: '352,342,300' 
in[   5][0.. 10]: '466,668,158' 
in[   6][0..  9]: '542,29,236' 
in[   7][0.. 10]: '431,825,988' 
in[   8][0.. 10]: '739,650,466' 
in[   9][0..  9]: '52,470,668' 
in[  10][0.. 10]: '216,146,977' 
in[  11][0..  9]: '819,987,18' 
in[  12][0.. 10]: '117,168,530' 
in[  13][0..  9]: '805,96,715' 
in[  14][0.. 10]: '346,949,466' 
in[  15][0..  9]: '970,615,88' 
in[  16][0.. 10]: '941,993,340' 
in[  17][0..  8]: '862,61,35' 
in[  18][0..  9]: '984,92,344' 
in[  19][0.. 10]: '425,690,689' 
```

We are to 'connect' junction boxes with strings of lights. But we must do this in a special way.

```sh
do 10 times
  find pair of closest connetions
    connect them
```

In the example.

1. The first pair that are closest together are 162,817,812 and 425,690,689 (node 0 and 19).
2. The next unconnected pair is 162,817,812 and 431,825,988 (node 0 and 7)
3. Next pair is 906,360,560 and 805,96,715 (node 2 and 13)
4. Then we find 431,825,988 and 425,690,689 (node 7 and 19)
   (Note that they have both already been picked. 0-7 and 0-19)
   ...

```text

      0 --- 19    2 -- 13
      |     |
      |     |
      7 ----/

```
    ...
11. When we have connected 10 pairs we have 11 curcuits.
    * One with 5 boxes
    * One with 4 boxes
    * Two with 2 boxes each
    * Seven remaining single box curcuits

We now multiply the size of the three largest curcuits (size 5,4,2) and get 40.

The full problem contains 1000 boxes. We are to connect 1000 pairs of increasing shortest distance apart. And then multiply the size of the three largest curcuits.

It seems we need to keep track of edges we have created? So that when we search for the next shortest possible edge to create we do not re-create one we already have?

But - we are allowed to create an edge between two nodes we have picked for previous edges?

So what would be an efficient way to find the next edge to create?

Key observations so far:

* We start with N unpaired nodes
* We are to connect them in edge-length order low to high
* An edge length is the eucledian distance 'square root of the sum of x,y,z diffs squared

## day8 part 2 Union Find on size (revisited)

This is an update to union find in 'Day 8 part 1 Union Find (first try)'.

We can use a Union Find on size. For part 2 we can not add N edges and then apply the Union fidn.

Observations:

- We are to add edges until the union count is one (all nodes connected).
- Thus we need to integrate the 'add edge' with 'Union Find'

Lets count the unions in our example.

```text

      0 --- 19    2 -- 13
      |     |
      |     |
      7 ----/

```

- Keep a vector 'parents' for each node that holds its parent node.
- Keep a vector 'counts' that holds the registered union member count (at root nodes)

- Initialise the 'parents' vector to make each node its own parent
- Initialise the 'counts' vector to all 'ones'
  (in effect make each node the single element in their own one element union)

My best shot in day8 part 2 was:

```c++
  // Union find on size (counts)
  std::vector<unsigned> parents(V,0);
  for (unsigned i=0;i<V;++i) parents[i] = i; // self parents = roots
  std::vector<unsigned> counts(V,1); // all unions 1 element

  auto find_root = [&parents](int x) {
    int current = x;
    while (parents[current] != current) {
      current = parents[current]; // back track
    }
    return current;
  };

  unsigned component_count = V; // Start with V single member unions
  for (unsigned e=0;e<E;++e) {
    auto const& [edge,dps] = we[e];
    auto const& [v1,v2] = edge;
    auto r1 = find_root(v1);
    auto r2 = find_root(v2);
    if (r1 == r2) continue; // already same union (same root)
    if (counts[r1] < counts[r2]) std::swap(r1,r2); // make '1' the larger one
    parents[r2] = r1; // same root (makes all childs of r2 also find root r1 == joined)
    counts[r1] += counts[r2]; // joint size
    --component_count;
    if (component_count==1) {
      last_edge = edge;
      break;
    }
  }
```

Come to think of it. For part 2 we don't need the union sizes?

## V*(V-1)/2 unique possible connections

```c++
  std::vector<size_t> sorted_edges(V*(V-1)/2);
  for (size_t i=0;i<V;++i) {
    for (size_t j=i+1;j<V;++j) {

    }
  }
```

Worth noting - The 'diagonal' (0,0),(1,1),...(V-1,V-1) has V members. So If we were to include them we ned the size V*(V+1)/2

```c++
  std::vector<size_t> sorted_edges(V*(V+1)/2);
  for (size_t i=0;i<V;++i) {
    for (size_t j=i;j<V;++j) {

    }
  }
```

## Day 8 part 1 Union Find (first try)

It seems we can apply a 'union find'?

*Disclaimer* The following is not fully correct! I realised this while solving part 1.

- Keep a vector that stores the 'root' of each node in our graph
- Keep a vector that stores a 'rank' of each node in our graph

Lets count the unions in our example.

```text

      0 --- 19    2 -- 13
      |     |
      |     |
      7 ----/

```

- We init the roots to themselves: roots [0,1,2,3,...,19]
- We init the ranks for all nodes to 1: ranks [1,1,1,...,1]
- We now go over the edges in our graph
- Edge 0-7: We update the root of rhs node 7 to the root of lhs node 0  

```text
                            1                   2
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
roots:  0   2         7           13          19
ranks:  1   1         1           1           1

Process edge: 0-7
* root of 7 = root of 0
* rank of root of 0 - increment


                            1                   2
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
roots:  0   2         0           13          19
ranks:  2   1         1           1           1
```

- Edge 2-13: Update the root of 13 to the root of 2

```text
                            1                   2
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
roots:  0   2         0           13          19
ranks:  2   1         1           1           1

Process edge: 2-13
* root of 13 = root of 2
* rank of root of 2 - increment

                            1                   2
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
roots:  0   2         0           2          19
ranks:  2   2         1           1           1

```

- Process edge 7-0: Already have the same roots = skip
- Process edge 7-19: Update the root of 19 to the root of 7

```text
                            1                   2
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
roots:  0   2         0           13          19
ranks:  2   1         1           1           1

Process edge: 7-19
* root of 19 = root of 7
* rank of root of 7 - increment

                            1                   2
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
roots:  0   2         0           2           0
ranks:  3   2         1           1           1

```

- Edge 19-0: Already have the same root 0 = skip.
- Edge 19-7: Already have the same root = 0 = skip.

We can now examine the roots and ranks vectors to determine the number of disjoint unions as well as their sizes.

```text
                            1                   2
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
roots:  0   2         0           2           0
ranks:  3   2         1           1           1
```

- Each union are now represented by a unique root.
- We have the roots 0 and 2 = two disjunct unions.
- The size of the union wih root 0 is ranks[0] = 3.
- The size of the union with root 2 is ranks[2] = 2.

This is actually NOT how the Union Find should work!

- When we join we need to operate on roots (top parent)
- When we count we also need to operate on roots (sum up counts for joined union roots)

Darn!

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

Each time it hits a splitter on its path it can chose left or right (if there is room).

So at each row we can keep a running count of the number of ways the particle can reach a column on that row.

# day 6 - part 2

There is actually something important to notice about the input.

```text
123 328  51 64 
 45 64  387 23 
  6 98  215 314
*   +   *   +  
```

Can you see what it is?

---

Each column does in fact have a fixed width!

```text
123 328  51 64 
 45 64  387 23 
  6 98  215 314
*   +   *   +  
```

What is the column with of the data un the example above?

---

We need to determine where each column starts and ends.

```text
123 328  51 64 
 45 64  387 23 
  6 98  215 314
*   +   *   +  
```

What algorithm can we use to find the column split positions?

---

The column splits are the positions that have a ' ' in all rows.

```text
123 328  51 64 
 45 64  387 23 
  6 98  215 314
*   +   *   +  
---------------
xxx xxx xxx xxx
   |   |   |
  s p l i t s
```
---

When we know the split positions we can create a table with rows fo extracted column fields

```text

// rows_of_column_fields

'123' '328' ' 51' '64 '
' 45' '64 ' '387' '23 '
'  6' '98 ' '215' '314'
'*  ' '+  ' '*  ' '+  '
```

How can we turn single digit columns right to left into single digit rows?

---

We can pivot the table counter-clockwise on single character columns!

```text
'123' '328' ' 51' '64 '
' 45' '64 ' '387' '23 '
'  6' '98 ' '215' '314'
'*  ' '+  ' '*  ' '+  '
```

```text
'  4 '
'431 '
'623+'
'175 '
'581 '
' 32*'
'8   '
'248 '
'369+'
'356 '
'24  '
'1  *'
```

DARN! The inout data is a GRID! I can just pivot the grid and be done with it?!

---
# day 3

## part 1

In part 1 we are to 'turn on' two batteries in a 'jolt battery bank' in such a way that the bank produces the 'highest jolt'.

The jolt produced by the bank is determined by combining the 'jolt value' for the two batteries 'as if' they combine into a decimal number jolt value. The first battery provides the first digit and the second battery the second digit of the combined jolt value.

We are not allowed to re-arrange the order of batteries in the bank.

So what is the 'highest jolt' we can get from a bank with batteries marked '987654321111111'?

Pause the video here if you want to think about this yourself.

Well, we see that if we turn on the battery marked '9' and then the battery marked '8' we get the jolt value 98. And this is the highest we can get by turning on two batteries in the bank.

So what is the 'highest jolt' we can get from a bank with batteries marked '818181911112111'?

Pause the video here if you want to think about this yourself.

In this case we see that if we pich battery marked '9' and then the one marked '2' whe get the jolt value 92 which is the hights we can get.

It seems we scan the string from left to right looking for the current largest first digit. And when we found one, we continue to find the following largest second digit.

## part 2

In part one we are to 'turn on' 12 batteries in a 'jolt battery bank' in such a way that the bank produces the 'highest jolt'.

The jolt produced by the bank is determined by combining the 'jolt value' for the turned on batteries 'as if' they combine into a decimal number jolt value. The first turned on battery provides the first digit, the second turned on battery the second digit, and so on.

We are not allowed to re-arrange the order of batteries in the bank.

So what is the 'highest jolt' we can get from a bank with batteries marked '987654321111111'? If we turn on 12 batteries and is not allowed to change the battry order?

Pause the video here if you want to think about this yourself.

The answer is 987 654 321 111 (987 billion 654 miljon 321 thousand 111).

We can view this as a 'turning off batteries' or 'remove digits' problem where we keep all digits but turn off the last three '1'.

```text
987654321111111
            xxx
987654321111
```

So what is the 'highest jolt' we can get from a bank with batteries marked '818 181 911 112 111'?

Pause the video here if you want to think about this yourself.

In this case the answer is 888 911 112 111 (888 billion 911 miljon 112 thousand 111).

Again we can view this as a 'turning off batteries' or 'remove digits' problem where we keep all digits but the three leftmost '1' in the sequnce.

```text
818181911112111
 x x x
8 8 8 911112111
```

It seems we scan the string from left to right looking for the largest first digit. Then, when we found one we continue to scan for the largest second digit and so on. With the caveat that if the digit we are processing is in fact even larger than the last picked one, we instead replace that one. In fact, we 'delete' all digits picked so far that are less than the current one in the input.

We can solve this problem using a 'monotonic decreasing stack'.

We create this stack by ensuring each digit we push onto the stack is always equal or less to the current top digit.

So, when we examine the next digit in the input, and we find that the top of the stack contains a digit that is less than the one we are about to push - then we purge digits from the stack until the top fullfills the criteria. Then we can safelly push the new one.

While doing this we also upholds the requirement to keep 12 digits. We do this by keeping track of how many digits we are allowed to delete. And when we pop a digit from the stack we are in fact 'deleting' it. Thus, we can pair the pop with decrementing the 'to delete' counter.

All left to do now is to empty the stack of digits so we have the 12 first digits we are looking for. Then dump the stacked digits to a string. And finally reverse it to get the digits in the correct order and transform it into a decimal number.

# day 2

## part 1

Consider the following problem. You are given a range of decimal numbers defined by a first and last number.

In this range, you are to find any number that is made up of two repeating sequence of digits.

For example, the number '22' is made up of the digit '2' repeated twice.

And the number '1188511885' is made up of the number '11885' repeated twice.

Consider the range 222220-222224.

```text
222220
222221
222222
222223
222224
```

How may numbers in this range is made up of one number repeated twice?

Pause the video here if you want some time to study this problem.

So, number 222222 is made up of 222 repeated twice.

It seems we can split the digit string i two equal length halves and compare if the two halfs are equal.

## part 2

Consider the following problem. You are given a range of decimal numbers defined by a first and last number.

In this range, you are to find any number that is made up of any repeated sequence of digits.

For example, the number '22' is made up of the digit '2' repeated twice.

And the number '824824824' is made up of the number '824' repeated three times.

Consider the range 2121212118-2121212124.

```text
2121212118
2121212119
2121212120
2121212121
2121212122
2121212123
2121212124
```

How may numbers in this range is made up of one number repeated two or more times?

Pause the video here if you want some time to study this problem.

So, number 2121212121 is made up of 21 repeated five times.

It seems a naive approach could be to split the strings into sequences of length 1 up to half the digit string. And then check if all sub-sequences are equal.

But digging a bit deeper into this problem we find that there is an algorithm called the [Knuth–Morris–Pratt algorithm](https://en.wikipedia.org/wiki/Knuth–Morris–Pratt_algorithm) that we can use for our problem.

The Knuth–Morris–Pratt algorithm, also called the 'KMP algorithm' defines a 'failure' function f: index -> length; that takes an index into the input string, and returns the length of the longest overlapping pattern that is both at the start and the end of the substring s[0..index].

The key insight here is 'longest overlap'!

Take the string '2121212121' (10 digits). The failure function f(9) (last index in the input string) will be 8. This means that the 8-digit string '21212121' is the longest overlapping string that the full input string both starts and ends with.

```text
   0123456789
s: 2121212121
   21212121     prefix
     21212121   suffix
     --------
     8 digits
```

From this, the next step is to calculate the 'gap' at the beginning and end for the pattern as prefix and suffix. Or put another way, how much we must translate the patterns from its prefix position to its suffix positions. We get this translation offset (gap) as the input string length minus the pattern length. Or Expressed using n as the input string length and the failure function: 

```text
gap = n - f(n-1).
```

Now we can make an interesting observation. If the found pattern that matches both as a prefix and suffix to the full input string, can be translated with the gap length and still match - then the pattern must be made up of repitions of digits with this exact length!

```text

s: 2121212121
   21212121     prefix
   --
   | 21212121   suffix
   | --
   | |
   | match
   |
   match

```

To build some intution we can study the string '1188511885'. In this case the failure function f(9) = 5.

```text
   0123456789
s: 1188511885
   11885      prefix
        11885 suffix
```

The 'gap' is now 10-5 = 5 (we get the suffix by translating the prefix 5 steps to the right). In this case we have no overlap.

The final conclusion is thus, that of the 'gap' length divides the input string length without reminders, then the string is fully made up of a pattern with the length of the gap. We get this pattern as the gap-size prefix of the pattern (and thus also the inout string).

# day 1

## part 1

In part one we are to count the number of times we stop at zero on a 0..99 marked knob, when we turn left or right a number of markings as instructed.

The input is one turn directive per line.

Each directive is a single capital letter—'L' for left or 'R' for right—immediately followed by the number of markings to turn.

I mapped the first character to +1 or -1 using a ternary expression, then parsed the remaining substring into an integer using C++ std::stoi.

Right from the start, I approached this as a modular arithmetic problem—stepping plus or minus, then wrapping the result into the 0..99 range using the modulus operation.

Since C++'s '%' operator is actually a signed remainder operation, I implemented a true mathematical modulus function to correctly handle the 0..99 range.

Then for part 1, I could simply count the number of times we ended up on the zero marking, while we went through the turn directions.

## part 2

In part 2 I was, for longer than I liked, stumped by the problem formulation. I failed to map it to some simple mathematical modulus operation.

But watching some YouTube videos after I solved it calmed me down. The ones I watched encountered the same bewilderment and confusion. It seems this was in fact a nice twist on the 'part one' puzzle!

I ended up solving it kind-of 'brute force' with each corner case taken care of in an if-else chain of statements.

I transformed my solution to track both 'current' and 'next' positions, where 'next' is the unwrapped value before applying modulus to the knob range. 

In this way I did not obfuscate the actual distance traveled. Which in turn enabled me to look at the next position to see how many times we would pass the zero position on the knob.

The way I did this was to check whether the next position is above zero, at zero, or below zero.

If we end up above zero, the number of times we pass zero equals the number of complete 100-mark intervals we travel. In C++, we get this using integer division on integer arguments.

If we end up at zero, this also counts as one click.

If we end up below zero, we pass zero at least once (since we start in the 0..99 range), plus once for each additional 100-mark interval. However, there's a corner case: if we started at zero, we need to subtract one to avoid double-counting the previous landing.

While this solution works correctly for all inputs, I'm still curious whether there's a more elegant mathematical approach using just the distance traveled. Let me know in the comments if you have ideas!

Warm thanks for watching, and see you in the next one!