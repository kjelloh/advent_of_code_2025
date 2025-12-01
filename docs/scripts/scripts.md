# day 01

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