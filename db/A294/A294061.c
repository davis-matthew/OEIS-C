/*
 * A294061  Sum of the differences of the larger and smaller parts in the
 *          partitions of n into two parts with the larger part squarefree.
 * https://oeis.org/A294061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (n-2i) * mu(n-i)^2 * (mu is the
 *            Möbius function; mu(x)^2 is 1 iff x is square‑free, else 0)
 * Range:     u8 n <= 39, u16 n <= 655, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- Integer square root ---------------------------------------------- */
static idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* --- Smallest‑square‑free sieve ----------------------------------------- */
static uint8_t *sqfree_cache = NULL;  /* 1 = square‑free, 0 = not */
static idx_t  sqfree_size   = 0;      /* highest index computed (inclusive) */

/* Ensure sqfree_cache contains values up to 'limit' (inclusive). */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sqfree_size)
        return;                     /* already done */

    /* Extend the cache, initializing new entries as square‑free. */
    uint8_t *new_cache = (uint8_t *)realloc(sqfree_cache, (size_t)(limit + 1) * sizeof(uint8_t));
    if (!new_cache)
        abort();                    /* malloc failure */
    for (idx_t i = sqfree_size + 1; i <= limit; ++i)
        new_cache[i] = 1;
    sqfree_cache = new_cache;

    /* Simple sieve of Eratosthenes to find primes up to the square root. */
    bool *prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!prime)
        abort();
    for (idx_t i = 2; i <= limit; ++i)
        prime[i] = true;
    prime[0] = false;
    prime[1] = false;

    idx_t maxp = isqrt_idx(limit);
    for (idx_t p = 2; p <= maxp; ++p) {
        if (prime[p]) {
            for (idx_t k = p * p; k <= limit; k += p)
                prime[k] = false;
        }
    }

    /* Using the primes up to sqrt(limit) to mark non square‑free numbers:
       any multiple of p*p (with p prime) is not square‑free. */
    for (idx_t p = 2; p <= maxp; ++p) {
        if (prime[p]) {
            idx_t pp = p * p;
            for (idx_t k = pp; k <= limit; k += pp)
                sqfree_cache[k] = 0;
        }
    }

    free(prime);
    sqfree_size = limit;
}

/* --- Sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                          /* undefined below offset */

    /* We need square‑free information up to n-1. */
    if (n > 1)
        ensure_sieve(n - 1);

    seq_t sum = 0;
    for (idx_t i = 1; i <= n / 2; ++i) {
        idx_t m = n - i;
        if (m <= sqfree_size && sqfree_cache[m]) {   /* mu(m)^2 == 1 ⇔ m square‑free */
            sum += (seq_t)n - (seq_t)2 * (seq_t)i;
        }
    }
    return sum;
}
