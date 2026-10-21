/*
 * A284816  Sum of entries in the first cycles of all permutations of [n].
 * https://oeis.org/A284816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n^2 + n + 2) / 4 Recurrence used: a(n) = a(n‑1) * n
 *            * (n^2 + n + 2) / (n^2 – n + 2), with gcd reductions to avoid
 *            overflow
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Cache for already computed terms */
static seq_t *cache = NULL;        /* array of values a(1)...a(k) */
static size_t cache_sz = 0;        /* number of stored values (k) */
static size_t cache_cap = 0;       /* capacity of the cache array */

/* Resize the cache if necessary */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) with caching and recurrence */
seq_t a(idx_t n)
{
    /* n >= 1 (offset 1) */
    if (n == 0)
        return (seq_t)0; /* undefined, but return 0 */

    /* Ensure cache can hold up to n */
    ensure_cache_capacity((size_t)n);

    /* Compute missing terms incrementally */
    for (idx_t i = (idx_t)(cache_sz + 1); i <= n; ++i)
    {
        seq_t value;
        if (i == 1)
        {
            value = (seq_t)1; /* a(1) = 1 */
        }
        else
        {
            seq_t prev = cache[i - 2]; /* a(i-1) */
            seq_t nn   = (seq_t)i;

            /* Compute i^2 once */
            seq_t ii = nn * nn;                /* i^2 */
            seq_t denom = ii - nn + (seq_t)2;      /* i^2 - i + 2 */
            seq_t numerator = nn * (ii + nn + (seq_t)2); /* i * (i^2 + i + 2) */

            /* Reduce division by gcds */
            seq_t g1 = gcd(prev, denom);
            seq_t a1 = prev / g1;
            seq_t denom1 = denom / g1;

            seq_t g2 = gcd(numerator, denom1);
            seq_t num2 = numerator / g2;
            seq_t denom2 = denom1 / g2; /* should become 1 */

            /* Final product */
            seq_t prod = a1 * num2;
            if (denom2 != (seq_t)1)
                prod /= denom2;

            value = prod;
        }
        cache[i - 1] = value; /* store a(i) */
    }
    cache_sz = (size_t)n;
    return cache[n - 1];
}
