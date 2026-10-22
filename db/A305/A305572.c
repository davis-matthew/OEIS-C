/*
 * A305572  a(n) = (-1)^(n-1) + Sum_{d|n, d>1} a(n/d)^d.
 * https://oeis.org/A305572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^(n-1) + Σ_{d|n, d>1} a(n/d)^d
 * Range:     u8 n <= 23, u16 n <= 35, u32 n <= 71, u64 n <= 143,
 *            u128 n <= 287, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

/* global memoisation arrays */
static seq_t *cache   = NULL;          /* computed a(n) values */
static unsigned char *flag = NULL;     /* 1 = value computed, 0 = not computed yet */
static size_t cache_cap = 0;           /* current capacity of cache */

/* Ensure arrays can address indices up to n */
static void ensure_storage(idx_t n)
{
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 128) new_cap = 128;   /* minimal initial size */
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        flag  = (unsigned char*)realloc(flag,  new_cap * sizeof(unsigned char));
        /* initialise new flag entries to 0 (undefined) */
        for (size_t i = cache_cap; i < new_cap; ++i) flag[i] = 0;
        cache_cap = new_cap;
    }
}

/* Recursive helper that actually evaluates a(n) */
static seq_t compute_a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* a(1) = 1 */

    ensure_storage(n);
    if (flag[(size_t)n]) return cache[(size_t)n];

    seq_t sum = 0;

    /* iterate over divisor pairs d, n/d, d >= 2 */
    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t nd = n / d;                         /* partner divisor */

            /* term for divisor d: a(n/d)^d */
            seq_t val_nd = compute_a(nd);
            sum += big_pow(val_nd, (seq_t)d);

            /* term for divisor nd (if >1 and not equal to d) */
            if (nd > 1 && nd != d) {
                seq_t val_d = compute_a(d);
                sum += big_pow(val_d, (seq_t)nd);
            }
        }
    }

    /* contrib from the divisor d = n itself: a(1)^n = 1^n = 1 */
    sum += (seq_t)1;

    /* add the (-1)^(n-1) contribution */
    if ((n & 1) == 0) {      /* n even → (-1)^(n-1) = -1 */
        sum -= (seq_t)1;
    } else {                 /* n odd  → (-1)^(n-1) = +1 */
        sum += (seq_t)1;
    }

    cache[(size_t)n] = sum;
    flag[(size_t)n]  = 1;
    return sum;
}

/* Public function required by the harness */
seq_t a(idx_t n)
{
    return compute_a(n);
}
