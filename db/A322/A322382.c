/*
 * A322382  a(n) = p*a(n/p) + 1, where p is the smallest prime divisor of n;
 *          a(1)=0.
 * https://oeis.org/A322382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 9573, u32 n <= 9573, u64 n <= 9573,
 *            u128 n <= 9573, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence from OEIS A322382:
   a(1) = 0;
   for n > 1, let p be the smallest prime divisor of n,
   then a(n) = p * a(n / p) + 1. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Allocate or grow a static cache to hold already computed values.
   Use the provided seq_t type directly. */
seq_t a(idx_t n)
{
    if (n <= 1)                /* a(1) = 0 */
        return (seq_t)0;

    /* Static cache arrays.  cache[0] corresponds to a(1). */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* number of elements held */

    /* Ensure the cache can hold up to n elements. */
    if ((idx_t)cap < n) {
        size_t newcap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();    /* allocation failure */
        /* Zero‑initialize any newly allocated slots. */
        for (size_t i = cap; i < newcap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cap   = newcap;
    }

    /* Return cached value if it has already been computed. */
    if (cache[n - 1] != (seq_t)0)
        return cache[n - 1];

    /* Find smallest prime divisor of n using trial division. */
    uint64_t m = (uint64_t)n;
    uint64_t p = 2ULL;
    if (m % 2ULL == 0ULL) {
        p = 2ULL;
    } else {
        for (uint64_t d = 3ULL; d <= m / d; d += 2ULL) {
            if (m % d == 0ULL) {
                p = d;
                break;
            }
        }
        if (p == 2ULL && n == 2ULL)
            p = 2ULL;                      /* n = 2 */
        if (p == 2ULL && n > 2ULL && m % 2 != 0)
            p = (uint64_t)n;               /* prime n */
    }
    /* If no divisor found in the loop, n is prime. */
    if (p == 2ULL && n > 2ULL && m % 2 != 0) p = (uint64_t)n;
    if (p == 2ULL && n == 2ULL) p = 2ULL;
    if (p == 0ULL) p = (uint64_t)n;           /* fallback for primes > 2 */

    idx_t reduced = n / (idx_t)p;
    seq_t val = (seq_t)p * a(reduced) + (seq_t)1;

    cache[n - 1] = val;
    return val;
}
