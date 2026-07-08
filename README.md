# CPP09 — Advanced Templates, STL, and Containers

This module focuses on the C++ Standard Template Library (STL): its containers, iterators, and algorithms, and on using them to implement two non-trivial algorithms (Reverse Polish Notation evaluation and the Ford-Johnson merge-insertion sort). This README documents what I built, what I learned about the STL along the way, and why I made the container choices I did.

---

## What is the STL?

The Standard Template Library is the part of the C++ standard library built around four cooperating pieces:

- **Containers** — data structures that store collections of objects (`vector`, `deque`, `list`, `map`, `set`, and others).
- **Iterators** — a generic, pointer-like way to walk through a container's elements without needing to know how that container is implemented internally.
- **Algorithms** — generic functions (`sort`, `find`, `accumulate`, `binary_search`, and so on) that operate through iterators, so the same algorithm works across many different container types.
- **Functors** — small callable objects used to customize algorithm behaviour (comparators, predicates).

The point of the design is decoupling: an algorithm doesn't need to know it's working on a `vector` versus a `list` — it only needs an iterator of the right category. This is also why some algorithms simply won't compile on some containers (for example `std::sort` requires random-access iterators, which `std::list` does not provide — `list` ships its own `.sort()` member function instead).

### Container categories

**Sequence containers** — ordered by position, not by value:
- `vector` — contiguous memory, O(1) random access, O(1) amortized `push_back`, O(n) insert/erase in the middle or at the front.
- `deque` — chunked memory, O(1) random access (slightly more overhead than vector), O(1) `push_front` *and* `push_back`, O(n) insert/erase in the middle.
- `list` — doubly linked list, O(1) insert/erase anywhere given an iterator, no random access at all (O(n) to reach an index), more memory overhead per element.

**Associative containers** — sorted by key, implemented as a self-balancing binary search tree (red-black tree), giving O(log n) insert/find/erase:
- `set` / `multiset` — unique vs. duplicate keys, no separate value.
- `map` / `multimap` — key-value pairs, unique vs. duplicate keys.

**Unordered associative containers** (C++11, not used here since this module targets C++98) — hash-table based, O(1) average operations but no ordering guarantee: `unordered_set`, `unordered_map`, and their multi- variants.

**Container adapters** — not containers in their own right, but restricted-interface wrappers around a real container underneath:
- `stack` (LIFO) — defaults to `deque`.
- `queue` (FIFO) — defaults to `deque`.
- `priority_queue` (max-heap by default) — defaults to `vector`, since heap operations need random access.

---

## Exercise 00 — BitcoinExchange

**Goal:** read an exchange-rate database (`data.csv`, date/rate pairs) and an input file (`date | value` lines), and for each valid line print `value * rate` using the exchange rate on that date — or the closest earlier date if there's no exact match.

**Container used:** `std::map<std::string, double>` for the database.

**Why:** the whole task is a "find the rate for this date, or the nearest earlier one" lookup, and `map` is built for exactly this. Storing `date -> rate` pairs keeps them automatically sorted by date (`std::string` comparison happens to sort `YYYY-MM-DD` dates correctly, since the format is fixed-width and lexicographic order matches chronological order). The lookup itself uses `std::map::lower_bound()`, which finds the first entry not less than the given date in O(log n) — if that's an exact match, done; otherwise, step one entry backward to get the closest earlier date. Using `map` here means I never had to write my own search or sorting logic — the container does both jobs for free.

**What I learned:** this was my first real use of an associative container instead of a sequence one, and it clicked why `map`'s sorted-by-key property is not just a nice side effect but the actual reason to reach for it — `lower_bound`/`upper_bound` only make sense on something that's guaranteed sorted.

---

## Exercise 01 — RPN (Reverse Polish Notation)

**Goal:** evaluate a Reverse Polish Notation expression given as a program argument (e.g. `"8 9 * 9 - 9 - 9 - 4 - 1 +"` → `42`), handling `+ - * /`, and printing `Error` on any malformed input (including division by zero and mismatched operand counts).

**Container used:** `std::stack<int, std::list<int> >` — a `stack` adapter, explicitly backed by `std::list` instead of its default `std::deque`.

**Why `stack`:** RPN evaluation is a textbook stack problem — push every number encountered, and on every operator, pop the top two values, apply the operator, push the result back. That's push/pop/top and nothing else, which is exactly the narrow interface `std::stack` exposes. Using a raw `vector` or `deque` directly would have worked too, but would have exposed a much wider interface (random access, iteration, insertion in the middle) than the algorithm actually needs — `stack` documents the intent of the code by construction.

**Why `list` as the backing container specifically:** `std::stack` only needs `push_back`, `pop_back`, and `back()` from whatever it wraps, so `vector`, `deque`, or `list` all qualify. I chose `list` here specifically because the subject's container-reuse rule ("the container(s) you used to validate this exercise will not be usable for the rest of this module") meant that whatever backed the stack here would become unavailable in PmergeMe. Since PmergeMe's binary-insertion logic genuinely needs random-access iterators (see below), I kept `vector` and `deque` free for that exercise and backed `stack` with `list` instead — a container `stack` supports natively, with zero change to any of the push/pop/top logic.

**What I learned:** adapters are a different concept from containers — `stack` doesn't store anything itself, it just narrows the interface of a container it holds internally. Swapping the backing container (`std::stack<int, std::list<int> >`) doesn't change behaviour at all, only the performance characteristics underneath, which don't matter here since the stack never grows large.

**One bug worth noting:** my first submission only accepted the expression as a single quoted argument (`argc != 2`). The subject's usage examples are all quoted (`./RPN "8 9 * ..."`), and the wording says the program takes the expression "as an argument" (singular) — so this matches the spec as written, though I initially also experimented with joining multiple unquoted arguments into one string to be more permissive, before reverting: unquoted input runs into shell-specific glob expansion issues (e.g. a bare `*` gets expanded by the shell before the program ever sees it) that have nothing to do with the program's own correctness, so I kept the stricter, spec-matching behaviour.

---

## Exercise 02 — PmergeMe (Ford-Johnson / merge-insertion sort)

**Goal:** implement the Ford-Johnson algorithm to sort a sequence of positive integers, using two different containers, and show that it completes in reasonable time on at least 3000 integers.

**Containers used:** `std::vector<int>` and `std::deque<int>`, each holding a complete, independent implementation of the same algorithm, with runtime compared between the two.

**Why these two:** the algorithm's core operation is a *bounded binary search insertion* — repeatedly checking `chain[mid]` against a value to find its correct insertion point. That requires random-access iterators (`operator[]` in O(1)), which rules out `list` outright — binary search on a container without random access degenerates to linear-time element access, defeating the entire point of the algorithm. `vector` and `deque` both provide that random access with the same interface, which also makes them a meaningful, apples-to-apples pair for timing comparison: any difference in performance reflects a genuine difference in memory layout (contiguous vs. chunked) rather than a difference in Big-O access pattern.

### The Ford-Johnson algorithm, step by step

The idea is to sort using close to the theoretical minimum number of comparisons (Knuth's bound), by being deliberate about both *what* gets compared and *in what order*.

1. **Pair up elements.** Split the input into pairs of two, comparing each pair once and always putting the smaller value first.
2. **Recursively sort the larger elements.** Pull out just the larger half of every pair and recursively run the same algorithm on that smaller set. This is the recursive step — it bottoms out once a sublist has one or zero elements.
3. **Rebuild the pairs in that sorted order.** Re-arrange the original pairs so their larger halves appear in the order the recursive sort produced. The smaller half of each pair stays glued to its own partner throughout — nothing about *which* small value belongs to *which* large value ever changes, only the order the pairs are visited in.
4. **Build a main chain and a pending list.** The larger halves, now sorted, become the "main chain" — for free, no extra work needed. The smaller halves become the "pending" list, still unsorted, still waiting to be inserted.
5. **Insert the pending elements using Jacobsthal-ordered, bounded binary search.** Rather than inserting the pending elements in plain order, they're inserted in an order derived from the Jacobsthal sequence (1, 3, 5, 11, 21, 43, ...). This ordering keeps each binary search's window size just under the next power-of-two threshold for as long as possible, which is exactly where binary search's comparison count increases — so it minimizes how many times that threshold gets crossed across the whole insertion process. Each pending element is also guaranteed to be smaller than the main-chain element it was originally paired with, so its binary search only needs to scan up to that known position, not the whole chain — a second, independent saving on top of the ordering trick.
6. **Insert the leftover, if any.** With an odd number of elements, one is left unpaired after step 1. It has no known partner and therefore no bound to exploit — it's inserted with an ordinary, full-range binary search at the very end.

### Binary insertion, concretely

```cpp
size_t left = 0, right = chain.size();   // or a known bound, for the pending elements
while(left < right)
{
    size_t mid = left + (right - left) / 2;
    if(chain[mid] < value)
        left = mid + 1;
    else
        right = mid;
}
chain.insert(chain.begin() + left, value);
```

Each loop iteration halves the search range, giving O(log n) comparisons instead of an O(n) linear scan — `left` converges on the exact index the value belongs at while preserving sort order. The "bounded" version used for pending elements is identical except `right` starts at the known safe limit described in step 5 instead of `chain.size()`, shrinking the search window and saving comparisons on top of what plain binary search already saves.

**What I learned:** most of the real difficulty here wasn't the recursion or the binary search individually — both are standard techniques — but the bookkeeping needed to keep the "bound" of each pending element accurate as the main chain grows with every insertion. Every insertion physically shifts every later element one position to the right, so every pending element's recorded bound has to be incremented whenever an insertion happens at or before it, or the bound silently goes stale and later searches become wrong. Getting this right, and verifying it by counting actual comparisons against Knuth's theoretical minimum, was the most instructive part of the exercise.

---

## Resources

- [Containers in C++ STL — GeeksforGeeks](https://www.geeksforgeeks.org/cpp/containers-cpp-stl/)
- [YouTube — STL container/iterator overview](https://www.youtube.com/watch?v=Ct8ykaKrKOA)
- [Reverse Polish Notation — Medium](https://medium.com/@interviewbuddies/reverse-polish-notation-b88524252960)
- [Ford-Johnson / merge-insertion sort — LinkedIn post](https://www.linkedin.com/posts/nerraou_ford-johnson-merge-insertion-sort-activity-7076577075712675840-AJ1l/)