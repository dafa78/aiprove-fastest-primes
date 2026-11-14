/*
 * fastest-primes: count primes <= N (1 <= N <= 5,000,000)
 *
 * Odd-only sieve of Eratosthenes over a byte array.
 *  - index i represents the odd number 2i+3, i in [0, (N-1)/2)
 *  - 2 is counted separately; even numbers never stored or sieved
 *  - marking starts at p*p and steps by p (odd multiples only)
 *  - input parsed with a single read(2) + manual digit loop (no scanf)
 *
 * Dependency-free C99, no external libraries.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    /* Fast input: one read, manual parse, tolerate leading/trailing ws. */
    char buf[32];
    ssize_t r = read(0, buf, sizeof(buf) - 1);
    if (r <= 0) return 0;
    buf[r] = '\0';

    char *p = buf;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
    long n = 0;
    while (*p >= '0' && *p <= '9') {
        n = n * 10 + (*p - '0');
        p++;
    }

    if (n < 2) {
        puts("0");
        return 0;
    }

    /* m = count of odd numbers in [3, n] */
    long m = (n - 1) / 2;
    unsigned char *comp = (unsigned char *)malloc((size_t)m);
    if (!comp) return 1;
    memset(comp, 0, (size_t)m);

    /* Integer sqrt(n) without libm. */
    long sq = 1;
    while ((sq + 1) * (sq + 1) <= n) sq++;

    for (long prime = 3; prime <= sq; prime += 2) {
        if (!comp[(prime - 3) / 2]) {
            long start = (prime * prime - 3) / 2;
            for (long j = start; j < m; j += prime)
                comp[j] = 1;
        }
    }

    /* 2 plus every unmarked odd. */
    long count = 1;
    for (long i = 0; i < m; i++)
        count += !comp[i];

    free(comp);
    printf("%ld\n", count);
    return 0;
}
