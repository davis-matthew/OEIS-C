/*
 * A241938  Number of length 2+4 0..n arrays with no consecutive five elements
 *          summing to more than 2*n.
 * https://oeis.org/A241938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 54, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A241938
   Recurrence (for n > 7):
   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5) - 7*a(n-6) + a(n-7)
   Initial values:
   a(1)=26, a(2)=218, a(3)=1043, a(4)=3599, a(5)=10031,
   a(6)=24052, a(7)=51570
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Ensure the cache of computed terms is available */
    static seq_t *cache = NULL;
    static size_t cap    = 0;   /* allocated size of cache */
    static idx_t max_n   = 0;   /* highest n for which cache[n-1] is valid */

    /* Initial values for n = 1..7 */
    static const seq_t init[7] = {
        26,      // a(1)
        218,     // a(2)
        1043,    // a(3)
        3599,    // a(4)
        10031,   // a(5)
        24052,   // a(6)
        51570    // a(7)
    };

    /* Handle the trivial case first */
    if (n <= 7) {
        /* Allocate minimal cache if not already done */
        if (cache == NULL) {
            cache = (seq_t *)malloc(7 * sizeof(seq_t));
            for (idx_t i = 0; i < 7; ++i) cache[i] = init[i];
            cap   = 7;
            max_n = 7;
        }
        return cache[n-1];
    }

    /* Lazy allocation if cache has never been created */
    if (cache == NULL) {
        cache = (seq_t *)malloc(7 * sizeof(seq_t));
        for (idx_t i = 0; i < 7; ++i) cache[i] = init[i];
        cap   = 7;
        max_n = 7;
    }

    /* Expand cache and compute missing terms if necessary */
    if (n > max_n) {
        /* Grow the allocated block if needed */
        if ((size_t)n > cap) {
            size_t new_cap = cap;
            while (new_cap < (size_t)n) new_cap = new_cap ? new_cap * 2 : 8;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cap   = new_cap;
        }
        /* Compute terms from max_n+1 up to n */
        for (idx_t i = max_n; i < n; ++i) {     /* i is zero‑based index */
            seq_t a1 = cache[i-1];  /* a(n-1) */
            seq_t a2 = cache[i-2];  /* a(n-2) */
            seq_t a3 = cache[i-3];  /* a(n-3) */
            seq_t a4 = cache[i-4];  /* a(n-4) */
            seq_t a5 = cache[i-5];  /* a(n-5) */
            seq_t a6 = cache[i-6];  /* a(n-6) */
            seq_t a7 = cache[i-7];  /* a(n-7) */

            /* Positive and negative contributions to avoid underflow */
            seq_t pos = 7*a1 + 35*a3 + 21*a5 + a7;
            seq_t neg = 21*a2 + 35*a4 + 7*a6;
            cache[i] = pos - neg;
        }
        max_n = n;
    }

    return cache[n-1];
}
