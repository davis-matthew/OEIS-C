/*
 * A261482  Number of set partitions of [n] into exactly six parts such that
 *          no part contains two elements with a circular distance less than
 *          three.
 * https://oeis.org/A261482
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 20, u64 n <= 36, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function:
   ∑_{k=0}^{13} q_k * a(n-k) = 0,
   with q = {1, -5, 5, -25, 74, 16, 185, -205, -401, -737,
             -440, 380, 576, 576}.
   (Only a(6) = 1 and the following seven values are
   supplied as initial conditions; all other terms for n<6 are
   defined to be 0.)
*/

#include <stdlib.h>

static const int coeff[14] = {
    1,     /* q0  - not used directly */
   -5,   /* q1 */
    5,   /* q2 */
   -25,  /* q3 */
   74,   /* q4 */
   16,   /* q5 */
  185,   /* q6 */
 -205,  /* q7 */
 -401,  /* q8 */
 -737,  /* q9 */
 -440,  /* q10 */
 380,   /* q11 */
 576,   /* q12 */
 576    /* q13 */
};

seq_t a(idx_t n)
{
    /* terms for n < 6 are defined to be 0 */
    if (n < 6)
        return 0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* initial allocation and first terms */
    if (!cache) {
        /* start with a small power‑of‑two capacity */
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache)
            return 0;           /* allocation failure fallback */

        /* initialise all entries to 0 */
        for (idx_t i = 0; i < cache_cap; ++i)
            cache[i] = 0;

        /* known values for n = 6 … 13 (offset 6) */
        static const seq_t init[14] = {
            0, 0, 0, 0, 0, 0, /* indices 0‑5 */
            1,                 /* a(6) */
            7,                 /* a(7) */
            42,                /* a(8) */
            210,               /* a(9) */
            985,               /* a(10) */
            4400,              /* a(11) */
            18944,             /* a(12) */
            80067              /* a(13) */
        };
        for (idx_t i = 0; i < 14; ++i)
            cache[i] = init[i];

        cache_size = 14;    /* up to index 13 inclusive */
    }

    /* grow cache if required */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;  /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp)
            return 0;       /* allocation failure fallback */
        /* initialise new part to 0 */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* return memoised value if already computed */
    if (n < cache_size)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 13) /* pre‑initialised values */
            continue;

        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        for (int k = 1; k <= 13; ++k) {
            idx_t idx = i - k;
            seq_t term = cache[idx];
            int    c   = coeff[k];
            unsigned int abs_c = (c >= 0) ? (unsigned int)c : (unsigned int)(-c);
            seq_t prod = (seq_t)(abs_c) * term;   /* multiplication with small integer */

            if (c >= 0)
                sum_pos += prod;
            else
                sum_neg += prod;
        }

        /* result = sum_neg - sum_pos  (guaranteed non‑negative) */
        cache[i] = sum_neg - sum_pos;
    }

    cache_size = n + 1;
    return cache[n];
}
