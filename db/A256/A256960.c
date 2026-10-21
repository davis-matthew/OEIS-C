/*
 * A256960  a(0)=1, a(1)=4; thereafter a(n) = a(n-2)+2*A055099(n-1)+2^(n-1).
 * https://oeis.org/A256960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 4 a(n) = a(n‑2) + 2*A055099(n‑1) + 2^(n‑1) for
 *            n > 1 Equivalently the coefficients of the rational generating
 *            function give a linear recurrence of order 4: a(n) = 4*a(n‑1) +
 *            a(n‑2) – 8*a(n‑3) – 4*a(n‑4) for n ≥ 4 (valid for all n ≥ 0; the
 *            initial values a(0)…a(3) are 1, 4, 11, 36)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of cached elements */

    /* Initialise cache with the first four terms */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();      /* allocation failure */
        cache[0] = (seq_t)1;      /* a(0) */
        cache[1] = (seq_t)4;      /* a(1) */
        cache[2] = (seq_t)11;     /* a(2) */
        cache[3] = (seq_t)36;     /* a(3) */
    }

    /* Grow cache if the requested index is beyond the current length */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failure */
        cache = tmp;

        /* Compute new terms iteratively */
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t term;
            if (i == 0) {
                term = (seq_t)1;
            } else if (i == 1) {
                term = (seq_t)4;
            } else if (i == 2) {
                term = (seq_t)11;
            } else if (i == 3) {
                term = (seq_t)36;
            } else {
                /* a(i) = 4*a(i‑1) + a(i‑2) – 8*a(i‑3) – 4*a(i‑4) */
                seq_t t1 = cache[i - 1] * (seq_t)4;
                seq_t t2 = cache[i - 2];
                seq_t t3 = cache[i - 3] * (seq_t)8;
                seq_t t4 = cache[i - 4] * (seq_t)4;
                term = t1 + t2;
                term -= t3;
                term -= t4;
            }
            cache[i] = term;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
