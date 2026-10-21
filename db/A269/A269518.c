/*
 * A269518  Lexicographically least sequence of nonnegative integers that
 *          avoids 3/2-powers.
 * https://oeis.org/A269518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9160, u16 n <= 9160, u32 n <= 9160, u64 n <= 9160,
 *            u128 n <= 9160, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by incremental search:
   For each n the smallest non‑negative integer that does not create a
   3/2‑power ending at position n is chosen. */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms. */
    static seq_t *seq = NULL;
    static size_t cap = 0;   /* allocated size */
    static size_t len = 0;   /* number of terms computed */

    /* Grow the cache if necessary. */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 16;
        while (newcap < (size_t)n + 1)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        if (!tmp) abort();
        seq = tmp;
        cap = newcap;
    }

    /* Compute terms up to n.  len holds number of terms already set. */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        /* Try candidates from 0 upwards. */
        for (seq_t cand = 0;; ++cand) {
            seq[(size_t)i] = cand;          /* provisional value */
            bool ok = true;

            /* Check all possible 3/2‑powers ending at i.            */
            /* A 3/2‑power of length 3*t:  x y x with |x| = |y| = t. */
            for (idx_t t = 1; 3 * t <= i + 1; ++t) {
                idx_t start = i + 1 - 3 * t;   /* first index of x */
                /* Compare x with the last block (indices start+2*t … i). */
                bool match = true;
                for (idx_t k = 0; k < t; ++k) {
                    if (seq[(size_t)(start + k)] !=
                        seq[(size_t)(start + 2 * t + k)]) {
                        match = false;
                        break;
                    }
                }
                if (match) {   /* 3/2‑power detected */
                    ok = false;
                    break;
                }
            }

            if (ok)
                break;         /* found the least admissible candidate */
        }
    }
    len = (size_t)n + 1;   /* update cached length */
    return seq[(size_t)n];
}
