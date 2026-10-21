/*
 * A268513  Numbers n such that bigomega(n) = bigomega(n*(n+1)+41).
 * https://oeis.org/A268513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sequence A268513 – numbers n such that bigomega(n) =
 *            bigomega(n*(n+1) + 41)
 * Range:     u8 n <= 6, u16 n <= 516, u32 n <= 9795, u64 n <= 9795,
 *            u128 n <= 9795, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* Cache of the found terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_checked = 1;          /* next integer to test */

    /* Keep filling the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t t = (seq_t)next_checked;                     /* candidate integer */
        seq_t omega_t = bigomega(next_checked);            /* Ω(t) */

        /* Euclidean expression: t*(t+1)+41. */
        seq_t eu = t * (t + (seq_t)1) + (seq_t)41;
        seq_t omega_eu = bigomega((idx_t)eu);             /* Ω(t*(t+1)+41) */

        if (omega_t == omega_eu) {                         /* a valid term */
            if (cache_len == cache_cap) {                  /* grow cache */
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(cache,
                                 new_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = t;
        }
        ++next_checked;                                    /* advance candidate */
    }

    return cache[(size_t)n - 1];
}
