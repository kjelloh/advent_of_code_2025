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

But columns do NOT always share width!

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
pivot.

```text
    4
4 3 1
6 2 3
+
1 7 5
5 8 1
  3 2
*
8
2 4 8
3 6 9
+
3 5 6
2 4  
1
*
```

We need to respect spaces in the digit columns!

- '123 328  51 64 ' splits into '123' '328' ' 51' '64 '
---

- split on single ' ' and respect ' ' in resulting tokens.
- pivot the table on character level.
- Filter white space from rows
- On number, push on stack.
- On operator, fold stacked numbers with operator and add to answer
- return answer

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