# fastest-primes — TESTLOG

Built and tested 2026-11-14 on this VM (x86_64, Ubuntu 24.04, gcc 13.3.0,
4 CPUs shared). Build: `cc -O3 -march=native -o primes primes.c`.

## Correctness — public samples (cases.json)

| N | expected | got | verdict |
|---|----------|-----|---------|
| 10 | 4 | 4 | PASS |
| 100 | 25 | 25 | PASS |
| 1000 | 168 | 168 | PASS |
| 100000 | 9592 | 9592 | PASS |
| 1000000 | 78498 | 78498 | PASS |
| 5000000 | 348513 | 348513 | PASS |

## Correctness — edge cases

| input | got | expected | verdict |
|-------|-----|----------|---------|
| 1 | 0 | 0 | PASS |
| 2 | 1 | 1 | PASS |
| 3 | 2 | 2 | PASS |
| 4 | 2 | 2 | PASS |
| 5 | 3 | 3 | PASS |
| `"  1000\n\n"` (leading/trailing whitespace) | 168 | 168 | PASS |
| `"1000000"` (no trailing newline) | 78498 | 78498 | PASS |

## Correctness — brute-force cross-check

Python reference sieve computing pi(n) for all n ≤ 100,000; compared against
`./primes` for every N in 1..2000 plus every 997th N up to 100,000
(2,099 values total): **0 mismatches**.

## Benchmarks — wall-clock (fork+exec+pipe, Python `time.perf_counter`, 3 runs each)

`-O3 -march=native` build:

| N | run 1 | run 2 | run 3 | best | median |
|---|-------|-------|-------|------|--------|
| 10 | 26.90 ms* | 1.17 ms | 1.31 ms | 1.17 ms | 1.31 ms |
| 1000 | 1.06 ms | 0.86 ms | 0.79 ms | 0.79 ms | 0.86 ms |
| 100000 | 1.44 ms | 1.34 ms | 1.26 ms | 1.26 ms | 1.34 ms |
| 1000000 | 2.12 ms | 1.89 ms | 2.13 ms | **1.89 ms** | **2.12 ms** |
| 5000000 | 8.32 ms | 6.84 ms | 6.63 ms | **6.63 ms** | **6.84 ms** |

\* First run includes cold page-cache / loader warm-up; steady state is ~1.2 ms.

Portable fallback build (`cc -O3`, no `-march=native`):

| N | best | median |
|---|------|--------|
| 1000000 | 2.14 ms | 2.20 ms |
| 5000000 | 7.35 ms | 9.73 ms |

## Variant comparison (N = 5,000,000, median)

| variant | median |
|---------|--------|
| byte-array odd-only sieve (submitted) | 6.84 ms |
| bitset odd-only sieve | 8.89 ms |

Bitset rejected: read-modify-write per mark is slower than byte writes here.
Process start-up (~0.8–1.2 ms) is a large share of total time at small N.
