/*
 * A391903  a(n) = Sum_{k=0..floor(n/2)} binomial(3*k,3*(n-2*k)).
 * https://oeis.org/A391903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 28, u64 n <= 54, u128 n <= 107,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Sum_{k=0..floor(n/2)} binomial(3*k, 3*(n-2*k))
 * Recurrence: a(n) = 3*a(n-2)+3*a(n-3)-3*a(n-4)+21*a(n-5)-2*a(n-6)+3*a(n-7)+3*a(n-8)+a(n-9)
 * Initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=1, a(5)=20, a(6)=2, a(7)=84, a(8)=85
 */

#include <stdlib.h>
#include "../utils/binom.h"

/* Compute the binomial‑sum definition for small n (≤8). */
static seq_t sum_binom(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k * 2 <= n; ++k)
    {
        idx_t a = 3 * k;
        idx_t b = 3 * (n - 2 * k);
        sum += binom(a, b);
    }
    return sum;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Cache of previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cached_last = (idx_t)-1;

    /* Initialise cache and base values on first call. */
    if (cache == NULL)
    {
        cache_cap = 16;                    /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i <= 8; ++i)
            cache[i] = sum_binom(i);
        cached_last = 8;
    }

    /* Return if already computed. */
    if (n <= cached_last)
        return cache[n];

    /* Expand capacity if necessary. */
    if ((size_t)(n + 1) > cache_cap)
    {
        size_t new_cap = cache_cap;
        while ((size_t)(n + 1) > new_cap)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = cached_last + 1; i <= n; ++i)
    {
        if (i <= 8)
        {
            /* Base case: use the original definition. */
            cache[i] = sum_binom(i);
        }
        else
        {
            /* Recurrence with only additions and one subtraction. */
            seq_t sum_pos = 3 * cache[i - 2] + 3 * cache[i - 3] +
                            21 * cache[i - 5] + 3 * cache[i - 7] +
                            3 * cache[i - 8] + cache[i - 9];
            seq_t sum_neg = 3 * cache[i - 4] + 2 * cache[i - 6];
            cache[i] = sum_pos - sum_neg;
        }
    }
    cached_last = n;
    return cache[n];
}
