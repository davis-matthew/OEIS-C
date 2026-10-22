/*
 * A385691  E.g.f. A(x) satisfies A(x) = exp( x*(A(x) + A(w*x) + A(w^2*x))/3
 *          ), where w = exp(2*Pi*i/3).
 * https://oeis.org/A385691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/3)} (3*k+1) * binomial(n-1,3*k) *
 *            a(3*k) * a(n-1-3*k) Initial values: a(0) = 1, and a(1) = a(2) =
 *            a(3) = 1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *seq_cache = NULL;      /* cache of computed terms */
static size_t   cache_cap   = 0;      /* current capacity of seq_cache */
static idx_t    last_computed = 0;    /* highest index computed so far */
static int      initialized   = 0;    /* flag whether cache was initialized */

/**
 * Compute a(n) from OEIS A385691.  The function is memoised so that
 * repeated calls with arbitrary n are handled in O(n^2) overall time,
 * storing all intermediate terms in a static cache.
 */
seq_t a(idx_t n)
{
    /* Initialise the cache with the first term a(0)=1 */
    if (!initialized) {
        seq_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!seq_cache) return (seq_t)0;          /* allocation failed */
        cache_cap = 1;
        seq_cache[0] = (seq_t)1;
        last_computed = 0;
        initialized = 1;
    }

    /* Grow the cache if it cannot hold up to n */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* reallocation failed */
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* If the requested term is already computed, return it */
    if (n <= last_computed)
        return seq_cache[n];

    /* Compute all terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        unsigned __int128 sum128 = 0;
        idx_t maxk = (i > 0) ? ((i - 1) / 3) : 0;

        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t t1 = 3 * k;               /* 3*k */
            idx_t t2 = i - 1 - t1;          /* i-1-3*k */

            seq_t bin = binomial(i - 1, t1);        /* binomial coefficient */

            /* use 128‑bit values for intermediate products to avoid overflow */
            unsigned __int128 prod = (unsigned __int128)seq_cache[t1] *
                                      (unsigned __int128)seq_cache[t2];
            unsigned __int128 term = prod *
                                     (unsigned __int128)bin *
                                     (unsigned __int128)(3 * k + 1);

            sum128 += term;
        }

        seq_cache[i] = (seq_t)sum128;       /* truncate to seq_t */
    }

    last_computed = n;
    return seq_cache[n];
}
