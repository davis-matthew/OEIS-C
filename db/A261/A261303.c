/*
 * A261303  a(n+1) = abs(a(n) - gcd(a(n), 3n+2)), a(1) = 1.
 * https://oeis.org/A261303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 Recurrence: a(n+1) = abs(a(n) - gcd(a(n), 3n+2)) for n
 *            ≥ 1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>     /* for malloc, realloc, abort */
#include <stdint.h>     /* for uint64_t */
#include <stdbool.h>    /* for bool */

/* Utility for greatest‑common‑divisor on seq_t values */
#include "../utils/gcd.h"

/* External linkage as required */
seq_t a(idx_t n);

/* Cache to store computed terms; extended lazily. */
static seq_t *cache = NULL;   /* cache[0] = a(1) */
static idx_t cache_max = 0;   /* highest index present in cache */

/* Ensure that values up to and including index n are computed. */
static void ensure_cache(idx_t n)
{
    if (n == 0) {               /* undefined; leave cache unchanged */
        return;
    }

    if (n <= cache_max) {
        return;
    }

    idx_t new_max = n;
    /* Reallocate to hold 'new_max' terms. The cast is needed for C++ */
    seq_t *new_cache = (seq_t*)realloc(cache, (size_t)new_max * sizeof(seq_t));
    if (!new_cache) {
        abort();                 /* out of memory */
    }
    cache = new_cache;

    /* Fill terms from cache_max+1 up to new_max */
    for (idx_t i = cache_max + 1; i <= new_max; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)1;
        } else {
            /* Previous term a(i) is stored at cache[i-2] */
            seq_t prev = cache[i - 2];

            /* Compute 3*(i-1)+2, using seq_t arithmetic to avoid overflow */
            seq_t next_term = (seq_t)(i - 1);
            next_term = next_term * (seq_t)3 + (seq_t)2;

            seq_t g = gcd(prev, next_term);

            /* Absolute difference */
            seq_t diff = (prev >= g) ? (prev - g) : (g - prev);

            cache[i - 1] = diff;
        }
    }

    cache_max = new_max;
}

/* Sequence function as required */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; return 0 for n == 0 to avoid UB */
    if (n == 0) {
        return (seq_t)0;
    }

    ensure_cache(n);
    return cache[n - 1];
}
