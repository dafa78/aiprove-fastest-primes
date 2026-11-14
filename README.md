# fastest-primes — AiProve Arena entry

Count the primes ≤ N (1 ≤ N ≤ 5,000,000), read from stdin, print the count.

## Approach

C99, dependency-free, single file `primes.c`:

- **Odd-only sieve of Eratosthenes** over a byte array: index `i` represents
  the odd number `2i+3`. Even numbers are never stored or sieved; 2 is counted
  separately. Memory for N = 5,000,000 is ~2.5 MB.
- Marking starts at `p*p` and steps by `p` (odd multiples only), for odd
  `p ≤ sqrt(N)`. Integer square root computed without libm.
- **Fast input**: a single `read(2)` plus a manual digit loop (no `scanf`),
  tolerating leading/trailing whitespace and a missing trailing newline.
- Output is exactly the count followed by `\n`.

A bitset variant was tried and rejected: read-modify-write per mark made it
slower than plain byte writes at this size (~8.9 ms vs ~6.8 ms at N = 5M).

## Build

```sh
cc -O3 -march=native -o primes primes.c
```

## Run

```sh
echo 5000000 | ./primes
# 348513
```

## Portable fallback

If the grader's machine does not support `-march=native` (different CPU), use:

```sh
cc -O3 -o primes primes.c
```

Measured on this VM: only ~0.3 ms slower at N = 1,000,000 and ~1–3 ms slower
at N = 5,000,000, so the fallback is safe. Do **not** use `-march=native`
binaries on a different machine — build on the grading machine instead.

## Requirements

- C compiler (`cc`/`gcc`/`clang`), C99, no external libraries.
- No network, no files, no secrets involved.
