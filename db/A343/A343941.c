/*
 * A343941  Number of strict integer partitions of 2*n with
 *          reverse-alternating sum 4.
 * https://oeis.org/A343941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A343941 -- Number of strict integer partitions of 2*n with reverse‑alternating sum 4
   Formula (recurrence, valid for n > 14):
     a(n) = a(n-1) + a(n-2) - a(n-4) - a(n-5) + a(n-6)
   Initial values (for 0 ≤ n ≤ 14):
     a(0)=0, a(1)=0, a(2)=1, a(3)=0, a(4)=1, a(5)=2, a(6)=3,
     a(7)=3, a(8)=4, a(9)=5, a(10)=7, a(11)=8, a(12)=10,
     a(13)=11, a(14)=14
   The implementation memoises results so each value is computed at most once.
*/
#include <stdlib.h>
#include <stddef.h>

/* Header expectation: external types seq_t and idx_t are defined by the build. */

seq_t a(idx_t n)
{
    /* Fixed initial values up to n = 14. */
    static const seq_t init[15] = {
        (seq_t)0, /* a(0) */
        (seq_t)0, /* a(1) */
        (seq_t)1, /* a(2) */
        (seq_t)0, /* a(3) */
        (seq_t)1, /* a(4) */
        (seq_t)2, /* a(5) */
        (seq_t)3, /* a(6) */
        (seq_t)3, /* a(7) */
        (seq_t)4, /* a(8) */
        (seq_t)5, /* a(9) */
        (seq_t)7, /* a(10) */
        (seq_t)8, /* a(11) */
        (seq_t)10,/* a(12) */
        (seq_t)11,/* a(13) */
        (seq_t)14 /* a(14) */
    };

    /* Return directly for the known range. */
    if (n <= 14)
        return init[n];

    /* Static cache for memoised values. */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;   /* largest n currently stored in cache */

    /* Initialise cache if not yet done. */
    if (cache == NULL) {
        /* Allocate space for 0 … 14 initially. */
        size_t cap = 15;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < cap; ++i)
            cache[i] = init[i];
        cache_max = 14;
    }

    /* Grow cache if the requested n is beyond the current maximum. */
    if (n > cache_max) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure: abort safely. */
            /* In a production build, you could handle this differently. */
            exit(EXIT_FAILURE);
        }
        cache = tmp;

        /* Compute missing values using the recurrence. */
        for (idx_t k = cache_max + 1; k <= n; ++k) {
            /* k ≥ 15, so all referenced indices are valid. */
            seq_t val = cache[k - 1] + cache[k - 2];
            val -= cache[k - 4];
            val -= cache[k - 5];
            val += cache[k - 6];
            cache[k] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
