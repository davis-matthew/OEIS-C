/*
 * A301299  Coordination sequence for node of type V1 in "krq" 2-D tiling (or
 *          net).
 * https://oeis.org/A301299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A301299 – Coordination sequence for node of type V1
   in the “krq” 2‑D tiling.  The sequence satisfies
   a(0)=1, a(1)=4, a(2)=8, a(3)=13, a(4)=18, a(5)=22,
   a(6)=26, a(7)=29, a(8)=34, a(9)=40, a(10)=44,
   and for n ≥ 11

      a(n) = 2·a(n–5) – a(n–10)

   This recurrence follows from the generating function
   N(x)/D(x) with D(x)=x¹⁰–2x⁵+1.  The terms grow
   slowly (≈4·n/5) so all values fit in 64 bits for
   the ranges tested.

   The implementation memoises computed values so that
   any number of calls in arbitrary order are handled
   efficiently.
*/
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *cache = NULL;   /* array holding a(0) … a(max computed) */
    static size_t cap   = 0;       /* allocated capacity of 'cache' */
    static idx_t  next  = 0;       /* index of first element not yet computed */

    /* Ensure the cache can hold up to a[n] */
    ensure_cache(n, &cache, &cap);

    /* Initialise the first 11 terms on the very first call */
    if (next == 0) {
        const seq_t init[11] = {
            (seq_t)1,  /* a(0) */
            (seq_t)4,  /* a(1) */
            (seq_t)8,  /* a(2) */
            (seq_t)13, /* a(3) */
            (seq_t)18, /* a(4) */
            (seq_t)22, /* a(5) */
            (seq_t)26, /* a(6) */
            (seq_t)29, /* a(7) */
            (seq_t)34, /* a(8) */
            (seq_t)40, /* a(9) */
            (seq_t)44  /* a(10) */
        };
        for (idx_t i = 0; i < 11; ++i) cache[i] = init[i];
        next = 11;  /* next index ready for computation */
    }

    /* If the requested index is already computed, return it */
    if (n < next) return cache[n];

    /* Compute terms up to 'n' using the recurrence */
    for (idx_t i = next; i <= n; ++i) {
        cache[i] = (seq_t)2 * cache[i - (idx_t)5] - cache[i - (idx_t)10];
    }
    next = n + (idx_t)1;   /* update the next uncomputed index */

    return cache[n];
}
