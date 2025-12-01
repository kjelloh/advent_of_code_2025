# day 01

## part 1

In part one we are to count the number of times we stop at zero on a 0..99 marked nob, when we turn left or right a number of markings as instructed.

The input is one turn directive per line.

And each direction is a single character cpaital 'L' for left and capital 'R' for right turns. And emmidietly followed by a decimal value of the number of markings to turn.

I chose to map the first character in the input to an integer with value +1 or -1. I did this with a ternary expression.

 And then parse the remaning sub-string into a positive integer value using C++ std::stoi (string to int).

From the getgo I transformed the problem into a modulus 'clock arithmentic' one. Meaning I applied the 'stepping' plus or minus and then 'wrapped' the result into the 0..99 range with the modulud operation.

Though, as I am writing in C++, the '%' operator is in fact a 'signed reminder' operation. So I implemented a 'true mathematical modulus function' to get the problem domain range 0..99 correctly.

Then for part 1, I could simply count the number of times we ended up on the zero marking while we went through the turn directions.

## part 2

In part 2 I was, for longer than I liked, stumped by the problem formulation. I failed to map it to some sime mathematical modulus operation?

But whatching some youtube videos after I solved it calmed me down. The ones I watched encountered the same bewilderment and confusion. Seems this was in fact a nice twist on the part one puzzle!

I ended up solving it kind-of 'brute force' with each corner case taken care of in an if-else chain of statements.

First I transformed the solution to use a 'current' and 'next' positions. Where the 'next' position was the value before wrapping it to the nob values. 

In this way I did not obfuscate the actual distance traveled. Which in turn enabled me to look at the next position to see how many times we would pass the zero position on the nob.

The way I did this was to look if the next position is above zero, at zero or below zero.

If we end up above zero. Then the number of times we pass zero on the nob is the number of whole 100 intervals we travel. We find this with the integer 'div' operation. And in C++ we epxress this as plain division on integer arguments.

If we end up at zero, this also counts as one click.

Finally, if we end up below zero, we pass zero at least once (as we start from within the nob-range 0..99). Then we pass zero each interval of 100. But - We need to check for the corner case where we started at zero. In this case we reduce the number of clicks with one that we have already counted.

I am still a bit weary about this solution. For one it feels a bit fragile and convolutet. Secondly I kind-of feel we maybe should have operated on the actual distance traveled (and not only on the next position)? Right now I will go with the result that this solution produces the right answer for the input and leave it at that?

Please comment below if you have anything to add to this observation of mine?