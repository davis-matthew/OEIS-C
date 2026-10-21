/*
 * A279794  Number of Goldbach partitions (p,q) of 2n such that |p-q| > n.
 * https://oeis.org/A279794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of Goldbach partitions (p,q) of 2n with |p-q| > n.
 *            This implementation counts each unordered pair once, by
 *            iterating over the larger prime p and checking the condition 2*p
 *            > 3*n
 * Range:     u8 n <= 7979, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static uint8_t *prime_sieve = NULL;  /* 1 = prime, 0 = composite */
static idx_t sieve_limit = 0;        /* maximum value for which prime_sieve is valid */

/* Build or extend sieve up to 'limit' (inclusive). */
static void build_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;                     /* already sufficient */

    free(prime_sieve);              /* free old sieve if any */

    size_t sz = (size_t)(limit + 1);
    prime_sieve = (uint8_t *)calloc(sz, sizeof(uint8_t));
    if (!prime_sieve)
        return;                     /* allocation failure (caller will handle) */

    for (idx_t i = 2; i <= limit; ++i)
        prime_sieve[i] = 1;          /* mark all as potential primes */

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!prime_sieve[i])
            continue;
        for (idx_t j = i * i; j <= limit; j += i)
            prime_sieve[j] = 0;      /* eliminate multiples */
    }

    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* sequence is defined for n ≥ 1 */

    idx_t m = 2 * n;               /* the even number to partition */
    idx_t thr_mul2 = 3 * n;        /* threshold for 2*p > 3*n */

    build_sieve(m);
    if (!prime_sieve)
        return 0;                  /* allocation failure */

    seq_t count = 0;

    for (idx_t p = 2; p < m; ++p) {
        if (!prime_sieve[p])
            continue;

        /* Must be the larger prime: 2*p > 3*n. */
        if (2 * p <= thr_mul2)
            continue;

        idx_t q = m - p;
        if (q < 2 || q > m)
            continue;              /* safety check */

        if (prime_sieve[q])
            ++count;
    }

    return count;
}
