/*
 * A283544  Number of 3 X n 0..1 arrays with no 1 equal to more than one of
 *          its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+7*a(n-2)+11*a(n-3)-6*a(n-4)+11*a(n-5)-2*a(n-6)
 *            (valid for n ≥ 7, offset 1: a(1)=8, a(2)=33, a(3)=127, a(4)=578,
 *            a(5)=2369, a(6)=10069)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first six terms directly. */
    static const seq_t init[6] = {
        (seq_t)8ULL,      /* a(1) */
        (seq_t)33ULL,     /* a(2) */
        (seq_t)127ULL,    /* a(3) */
        (seq_t)578ULL,    /* a(4) */
        (seq_t)2369ULL,   /* a(5) */
        (seq_t)10069ULL   /* a(6) */
    };

    if (n <= 6) {
        return init[n-1];
    }

    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;          /* number of terms stored (>=6) */

    /* Initialise the cache on first use. */
    if (cache == NULL) {
        /* Allocate space for at least the first six terms. */
        cache = (seq_t *)realloc(NULL, (size_t)6 * sizeof(seq_t));
        for (idx_t i = 0; i < 6; ++i) {
            cache[i] = init[i];
        }
        max_cached = 6;
    }

    /* Grow the cache if necessary. */
    if (n > max_cached) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failure: abort gracefully (short‑circuit). */
            return (seq_t)0;
        }
        cache = tmp;

        /* Compute new terms using the recurrence. */
        for (idx_t k = max_cached + 1; k <= n; ++k) {
            idx_t idx = k - 1;              /* zero‑based index in cache */
            /* a(k) = 2*a(k-1)+7*a(k-2)+11*a(k-3)-6*a(k-4)+11*a(k-5)-2*a(k-6) */
            seq_t t = 2 * cache[idx-1]
                    + 7 * cache[idx-2]
                    + 11 * cache[idx-3];
            t -= 6 * cache[idx-4];
            t += 11 * cache[idx-5];
            t -= 2 * cache[idx-6];
            cache[idx] = t;
        }
        max_cached = n;
    }

    return cache[n-1];
}
