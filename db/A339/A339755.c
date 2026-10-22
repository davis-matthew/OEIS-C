/*
 * A339755  a(1) = 1; a(n+1) = 1 + Sum_{d|n} a(n/d) * a(d).
 * https://oeis.org/A339755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A339755
   a(1) = 1
   a(n+1) = 1 + Σ_{d|n} a(d) * a(n/d),  for n ≥ 1
   (i.e. a(n) = 1 + Σ_{d|n-1} a(d) * a((n-1)/d),  with a(1)=1) */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_realloc.h"

/* static cache */
static seq_t *cache        = NULL;  /* array of a(0)...a(max)    */
static size_t cache_cap    = 0;     /* allocated size (in elements) */
static size_t cache_last   = 0;     /* highest index already computed */

/* ensure array has room for indices up to needed-1 */
static void ensure_capacity_to(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = needed;
    if (new_cap < 2) new_cap = 2;
    /* grow at least by factor 2 to keep O(n) amortized time */
    if (cache_cap > 0) {
        while (new_cap < needed) new_cap <<= 1;
        if (new_cap < needed) new_cap = needed;
    }
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    cache      = tmp;
    cache_cap  = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* unused, but harmless */

    /* initialise cache once */
    if (cache == NULL) {
        ensure_capacity_to(2);      /* indices 0 and 1 */
        cache[0] = (seq_t)0;        /* unused */
        cache[1] = (seq_t)1;        /* a(1) = 1 */
        cache_last = 1;
    }

    /* if requested index already computed, return it */
    if ((size_t)n <= cache_last) return cache[(size_t)n];

    /* make sure we have space for the new index */
    ensure_capacity_to((size_t)n + 1);

    /* compute missing values up to n */
    for (size_t v = cache_last + 1; v <= (size_t)n; ++v) {
        size_t m = v - 1;            /* n-1 in recurrence */
        seq_t sum = (seq_t)0;

        /* iterate over all divisors d of m */
        for (size_t d = 1; (idx_t)d * (idx_t)d <= (idx_t)m; ++d) {
            if (m % d == 0) {
                size_t e = m / d;
                seq_t prod = seq_mul(cache[d], cache[e]);
                sum += prod;          /* contribution of (d,e) */
                if (d != e) {
                    sum += prod;      /* contribution of (e,d) */
                }
            }
        }
        cache[v] = (seq_t)1 + sum;
    }

    cache_last = (size_t)n;
    return cache[(size_t)n];
}
