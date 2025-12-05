---
marp : true
---
# Advent of Code - Day 5: Cafeteria

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
So, in this example, 3 of the available ingredient IDs are fresh.
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
  if (any_of(fresh_ranges) contains id) increment count
```

<!-- 
We should be able to count the number of fresh ingredients as expressed as follows. For each ingredient id, if any of the ranges degining fresh ingredient IDs contains this id, then increment the fresh ingredient counter.
-->

---
