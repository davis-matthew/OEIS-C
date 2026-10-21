/*
 * A280111  Indices of triangular numbers (A000217) that are also centered
 *          10-gonal numbers (A062786).
 * https://oeis.org/A280111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 39*a(n-1) - 39*a(n-2) + a(n-3) for n>3 * a(1) = 1, a(2) =
 *            58, a(3) = 2221
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for the sequence, return 0 */

    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t len = 0;   /* number of terms currently stored */
    static size_t cap = 0;   /* allocated capacity */

    /* ensure the cache contains at least n terms */
    if (len < n) {
        /* if uninitialized, prepare the first three values */
        if (len == 0) {
            size_t init = 3;
            cache = (seq_t *)malloc(init * sizeof(seq_t));
            if (!cache) return (seq_t)0; /* allocation failure */
            cache[0] = (seq_t)1;          /* a(1) */
            cache[1] = (seq_t)58;         /* a(2) */
            cache[2] = (seq_t)2221;       /* a(3) */
            len = 3;
            cap = init;
        }

        /* grow cache while needed */
        while (len < n) {
            /* compute next term via recurrence */
            seq_t term = (seq_t)39 * cache[len-1]
                        - (seq_t)39 * cache[len-2]
                        + cache[len-3];
            /* ensure capacity */
            if (len == cap) {
                size_t newcap = cap * 2;
                if (newcap < len + 1) newcap = len + 1;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* allocation failure */
                cache = tmp;
                cap = newcap;
            }
            cache[len] = term;
            len++;
        }
    }

    return cache[n-1];   /* zero‑based indexing in the cache */
}
