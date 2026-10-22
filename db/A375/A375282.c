/*
 * A375282  Expansion of (1 - x - x^4)/((1 - x - x^4)^2 - 4*x^5).
 * https://oeis.org/A375282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   alloc_seq_array, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A375282
 *
 * Formula (recurrence):
 *   a(0) = 1, a(1) = 1, a(2) = 1, a(3) = 1, a(4) = 2, a(5) = 7,
 *   a(6) = 16, a(7) = 29
 *   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-4) + 2*a(n-5) - a(n-8)   (n >= 8)
 *
 * The implementation keeps a cache of already computed terms.  It
 * allocates the array with the helper function:
 *   seq_t *alloc_seq_array(size_t n);
 * and frees old arrays with
 *   void free_cache(seq_t *p);
 * The code works for both the integer and arbitrary‑precision
 * implementations of seq_t and obeys the constraints set in the
 * problem description.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_cache.h"

/* static cache: cache[i] holds a(i) for 0 <= i < cap */
static seq_t *cache = NULL;   /* pointer to array of seq_t */
static idx_t  cap      = 0;   /* allocation capacity */
static idx_t  computed = 0;   /* number of entries already filled (>= 0) */

/* allocate or grow the cache to be able to address index n */
static void ensure_alloc(idx_t n)
{
    /* first allocation: 16 entries (overkill for small n) */
    if (!cache) {
        cap = 16;
        cache = alloc_seq_array(cap);
        if (!cache) return;              /* allocation failed */

        /* initialise the first eight terms */
        cache[0] = cache[1] = cache[2] = cache[3] = (seq_t)1;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)7;
        cache[6] = (seq_t)16;
        cache[7] = (seq_t)29;

        computed = 8;                    /* terms 0…7 are now known */
    }

    /* grow the cache if we need to reach index n */
    if (n + 1 > cap) {
        idx_t new_cap = cap;
        while (n + 1 > new_cap) new_cap <<= 1;

        seq_t *new_cache = alloc_seq_array(new_cap);
        if (!new_cache) return;          /* allocation failed */

        /* copy existing values to the new array */
        for (idx_t i = 0; i < computed; ++i)
            new_cache[i] = cache[i];

        free_cache(cache);               /* clean up old storage */
        cache = new_cache;
        cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_alloc(n);
    if (n + 1 > computed) {                    /* compute any missing terms */
        for (idx_t i = computed; i <= n; ++i) {
            if (i < 8) continue;         /* base values already set */

            /* recurrence:
                 a(i) = 2*a(i-1) - a(i-2) + 2*a(i-4) + 2*a(i-5) - a(i-8)
            */
            seq_t v = cache[i-1];
            v += cache[i-1];                 /* 2*a(i-1)  */
            v -= cache[i-2];                 /* -a(i-2)   */
            v += cache[i-4];
            v += cache[i-4];                 /* +2*a(i-4) */
            v += cache[i-5];
            v += cache[i-5];                 /* +2*a(i-5) */
            v -= cache[i-8];                 /* -a(i-8)   */

            cache[i] = v;
        }
        computed = n + 1;
    }

    return cache[n];
}
