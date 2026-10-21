/*
 * A270218  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 129", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=1, a(1)=4, a(2)=28, a(3)=140; for n≥4,
 *            a(n)=7*a(n-1)-14*a(n-2)+8*a(n-3) (OEIS A270218)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mul_small.h"   /* seq_t mul_small(seq_t x, unsigned int m) */

static seq_t *cache = NULL;          /* cache of computed terms             */
static size_t cache_cap = 0;         /* current capacity of the cache       */
static idx_t  last_computed = 0;     /* last index that has a value        */

/* ensure that CACHE can hold indices up to N (inclusive) */
static void
ensure_cache(idx_t N)
{
    size_t needed = (size_t)N + 1;
    if (needed <= cache_cap)
        return;
    seq_t *newp = NULL;
    if (cache == NULL)
        newp = (seq_t *)malloc(needed * sizeof(seq_t));
    else
        newp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (newp == NULL)
        abort();          /* allocation error: terminate */
    cache = newp;
    /* zero newly allocated slots (unnecessary for big‑ints but harmless) */
    for (size_t i = cache_cap; i < needed; ++i)
        cache[i] = 0;
    cache_cap = needed;
}

/* compute a(n) */
seq_t
a(idx_t n)
{
    /* initialise base values on first call */
    if (last_computed == 0) {
        ensure_cache(3);                 /* need indices 0…3 at minimum   */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)28;
        cache[3] = (seq_t)140;
        last_computed = 3;               /* highest computed index so far */
    }

    if (n <= last_computed)
        return cache[n];

    ensure_cache(n);                    /* enlarge cache for new indices */

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t t1 = mul_small(cache[i - 1], 7);     /* 7*a(i-1)   */
        seq_t t2 = mul_small(cache[i - 2], 14);    /* 14*a(i-2)  */
        seq_t t3 = mul_small(cache[i - 3], 8);     /* 8*a(i-3)   */
        cache[i] = t1 - t2 + t3;
    }

    last_computed = n;
    return cache[n];
}
