/*
 * A364539  G.f. satisfies A(x) = 1 + x*A(x) + x^3*A(x)^5.
 * https://oeis.org/A364539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(n+2k,5k) * binomial(5k,k) /
 *            (4k+1) (OEIS A364539)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Cached results of the sequence */
static seq_t *cache = NULL;          /* pointer to cached values          */
static size_t cache_cap = 0;         /* allocated length of cache        */

/* Compute a(n) for arbitrary n, memoising results for future calls. */
seq_t a(idx_t n)
{
    /* Allocate and compute up to n if this is the first call.              */
    if (!cache) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed – return indeterminate value */

        cache_cap = new_cap;
        for (idx_t i = 0; i <= n; ++i) {
            seq_t sum  = 0;
            idx_t kmax = i / 3;
            for (idx_t k = 0; k <= kmax; ++k) {
                seq_t c1 = binomial(i + 2 * k, 5 * k);
                seq_t c2 = binomial(5 * k,     k);
#if defined(__SIZEOF_INT128__)   /* C implementation with 128‑bit intermediate */
                unsigned __int128 prod128 = (unsigned __int128)c1 *
                                           (unsigned __int128)c2;
                seq_t term = (seq_t)(prod128 / (unsigned __int128)(4 * k + 1));
#else                                 /* C implementation without 128‑bit type   */
                seq_t prod  = c1 * c2;
                seq_t term  = prod / (4 * k + 1);
#endif
                sum += term;
            }
            cache[i] = sum;
        }
        return cache[n];
    }

    /* Extend cache if the requested n is beyond the current capacity.     */
    if (cache_cap <= (size_t)n) {
        size_t old_cap = cache_cap;
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* realloc failed – return indeterminate value */
        cache = tmp;
        cache_cap = new_cap;

        /* Compute only the newly required terms.                           */
        for (idx_t i = old_cap; i <= n; ++i) {
            seq_t sum  = 0;
            idx_t kmax = i / 3;
            for (idx_t k = 0; k <= kmax; ++k) {
                seq_t c1 = binomial(i + 2 * k, 5 * k);
                seq_t c2 = binomial(5 * k,     k);
#if defined(__SIZEOF_INT128__)
                unsigned __int128 prod128 = (unsigned __int128)c1 *
                                           (unsigned __int128)c2;
                seq_t term = (seq_t)(prod128 / (unsigned __int128)(4 * k + 1));
#else
                seq_t prod  = c1 * c2;
                seq_t term  = prod / (4 * k + 1);
#endif
                sum += term;
            }
            cache[i] = sum;
        }
    }

    return cache[n];
}
