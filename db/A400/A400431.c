/*
 * A400431  G.f. A(x) satisfies A(x) = 1 + 2*x*A(2*x)^(1/2).
 * https://oeis.org/A400431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A400431
   Recurrence:
     a(0) = 1, a(1) = 2;
     a(n) = (1/4) * ( 2^(n+1) * a(n-1)
                      - Σ_{k=2}^{n-1} a(k) * a(n+1-k) )   (n≥2)
   Rewritten to avoid overflow:
     a(n) = 2^(n-1) * a(n-1) - (1/4) * Σ_{k=2}^{n-1} a(k) * a(n+1-k)   (n≥2)
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  static storage for memoisation                                      */
/* --------------------------------------------------------------------- */
static seq_t *cache = NULL;      /* values a[0 .. cache_size-1]          */
static size_t cache_cap = 0;     /* capacity of cache array              */
static size_t cache_size = 0;    /* number of computed terms             */

/* --------------------------------------------------------------------- */
/*  helper: ensure cache can hold at least (n+1) entries                 */
/* --------------------------------------------------------------------- */
static inline void ensure_cache_cap(idx_t n) {
    size_t needed = (size_t)n + 1U;
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16U;
    while (new_cap < needed) new_cap <<= 1U;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*  main function                                                        */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* allocate cache if necessary */
    ensure_cache_cap(n);

    /* Calculate missing values in increasing order */
    idx_t start = cache_size;
    if (start != 0 && start != 1) { /* cache already has 0/1 if start>=2 */
        /* nothing to do */
    } else {
        /* initialize missing base cases */
        size_t up_to = (start == 0) ? 1 : 1;
        for (idx_t cur = start; cur <= up_to; ++cur) {
            cache[cur] = (cur == 0) ? (seq_t)1 : (seq_t)2;
        }
        cache_size = up_to + 1;
        start = cache_size;
    }

    /* If everything is ready, return */
    if (start > n) return cache[(size_t)n];

    /* Determine pow2_half = 2^(cur-1) for the first iteration (cur=start) */
    seq_t pow2_half = (seq_t)1;
    /* Compute pow2_half for the starting cur if it is >=2 */
    if (start >= 2) {
        for (idx_t i = 0; i < start - 1; ++i)
            pow2_half *= (seq_t)2;
    } else {
        /* start is 0 or 1, pow2_half value will not be used until cur==2 */
        pow2_half = (seq_t)2;   /* value for cur==2 */
    }

    /* Iterate over missing indices */
    for (idx_t cur = start; cur <= n; ++cur) {
        if (cur == 0) {
            cache[0] = (seq_t)1;
        } else if (cur == 1) {
            cache[1] = (seq_t)2;
        } else {
            /* term1 = 2^(cur-1) * a(cur-1) */
            seq_t term1 = pow2_half * cache[cur - 1];

            /* sum of products a(k)*a(cur+1-k) for k=2..cur-1 */
            seq_t sum = (seq_t)0;
            for (idx_t k = 2; k < cur; ++k) {
                sum += cache[k] * cache[cur + 1 - k];
            }
            /* a(cur) = term1 - sum/4 */
            cache[cur] = term1 - sum / (seq_t)4;

            /* Update pow2_half for next iteration: 2^(cur+0) = 2 * 2^(cur-1) */
            pow2_half *= (seq_t)2;
        }
    }

    cache_size = (size_t)n + 1U;
    return cache[(size_t)n];
}
