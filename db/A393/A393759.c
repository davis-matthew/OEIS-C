/*
 * A393759  The total number of at-most-6-furcating labeled histories that can
 *          be constructed for n labeled leaves.
 * https://oeis.org/A393759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n*(n-1)/2)*a(n-1) + (n*(n-1)*(n-2)/6)*a(n-2) +
 *            (n*(n-1)*(n-2)*(n-3)/24)*a(n-3) +
 *            (n*(n-1)*(n-2)*(n-3)*(n-4)/120)*a(n-4) +
 *            (n*(n-1)*(n-2)*(n-3)*(n-4)*(n-5)/720)*a(n-5) for n>=6 with
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=4, a(4)=29, a(5)=336
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_array.h"

/* compute binomial coefficient C(n,k) for 0 <= k <= 6 using multiplicative formula */
static inline seq_t binom_n_k(idx_t n, idx_t k)
{
    seq_t prod = 1;
    for (idx_t i = 0; i < k; ++i)
        prod *= (seq_t)(n - i);           /* (n)(n-1)…(n-k+1) */
    /* divide by k! ; k! fits in native 64-bit for k <= 6 */
    switch (k) {
        case 0: return 1;
        case 1: return prod;               /* 1! = 1 */
        case 2: return prod / 2;           /* 2! = 2 */
        case 3: return prod / 6;           /* 3! = 6 */
        case 4: return prod / 24;          /* 4! = 24 */
        case 5: return prod / 120;         /* 5! = 120 */
        case 6: return prod / 720;         /* 6! = 720 */
        default: return 0;                 /* should not happen */
    }
}

/* global cache of computed values */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* ensure cache has room for items 0 … n; return old capacity before expansion */
static idx_t ensure_cache(idx_t n)
{
    if (cache_cap > n) return cache_cap;
    idx_t old_cap = cache_cap;
    idx_t new_cap = n + 1;
    seq_t *new_cache = seq_alloc_array(new_cap);
    if (!new_cache) return old_cap;            /* allocation failed – caller will get garbage */

    for (idx_t i = 0; i < old_cap; ++i)
        new_cache[i] = cache[i];
    for (idx_t i = old_cap; i < new_cap; ++i)
        new_cache[i] = 0;

    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
    return old_cap;
}

seq_t a(idx_t n)
{
    /* base values for n = 0 … 5 */
    static const seq_t base[6] = {0, 1, 1, 4, 29, 336};

    /* ensure cache large enough, remember where to start filling */
    idx_t start = ensure_cache(n);
    if (start > n) return cache[n];  /* nothing to compute */

    for (idx_t i = start; i <= n; ++i) {
        if (i <= 5) {                /* use provided base values */
            cache[i] = base[i];
        } else {                     /* recurrence for i >= 6 */
            seq_t val = 0;
            val += binom_n_k(i, 2) * cache[i-1];
            val += binom_n_k(i, 3) * cache[i-2];
            val += binom_n_k(i, 4) * cache[i-3];
            val += binom_n_k(i, 5) * cache[i-4];
            val += binom_n_k(i, 6) * cache[i-5];
            cache[i] = val;
        }
    }
    return cache[n];
}
