/*
 * A339117  Number of cycles in the grid graph P_5 X P_n.
 * https://oeis.org/A339117
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A339117 – Number of cycles in the grid graph P_5 × P_n
 *
 *  The sequence satisfies the linear recurrence with a linear term
 *
 *      a(n) = 13 a(n-1) – 45 a(n-2) + 66 a(n-3) – 17 a(n-4)
 *             – 209 a(n-5) + 151 a(n-6) + 140 a(n-7)
 *             – 112 a(n-8) – 48 a(n-9) + 50 a(n-10) + 28 a(n-11)
 *             +  88 n    – 1488                     (n ≥ 13)
 *
 *  Initial values (index 2 … 12):
 *      10, 108, 1049, 9349, 80626, 692194, 5948291,
 *      51139577, 439673502, 3779989098, 32497334055.
 *
 *  The function memoizes computed terms so that multiple calls in
 *  arbitrary order take total time O(max n).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Indices below the sequence offset are undefined. */
    if (n < 2)
        return 0;  /* placeholder */

    /* Memoization table.  cache[i] holds a(i).  cache[0] and cache[1]
       are unused but allocated for simplicity. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t cache_max = 1;   /* highest index computed so far */

    /* Initial values for indices 2 … 12. */
    static const seq_t init_vals[11] = {
        10ULL,               /* a( 2) */
        108ULL,              /* a( 3) */
        1049ULL,             /* a( 4) */
        9349ULL,             /* a( 5) */
        80626ULL,            /* a( 6) */
        692194ULL,           /* a( 7) */
        5948291ULL,          /* a( 8) */
        51139577ULL,         /* a( 9) */
        439673502ULL,        /* a(10) */
        3779989098ULL,       /* a(11) */
        32497334055ULL       /* a(12) */
    };

    /* Coefficients for the recurrence.  Positive and negative
       contributions are separated to avoid intermediate negative
       seq_t values. */
    static const idx_t pos_off[6]  = {1, 3, 6, 7, 10, 11};
    static const seq_t pos_coef[6] = {13ULL, 66ULL, 151ULL, 140ULL, 50ULL, 28ULL};

    static const idx_t neg_off[5]  = {2, 4, 5, 8, 9};
    static const seq_t neg_coef[5] = {45ULL, 17ULL, 209ULL, 112ULL, 48ULL};

    /* Allocate initial cache and store the first 11 terms. */
    if (cache == NULL) {
        cache_size = (size_t)12 + 1;              /* indices 0 … 12 */
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (cache == NULL)                      /* allocation failed */
            return 0;

        for (idx_t i = 0; i < 11; ++i)
            cache[2 + i] = init_vals[i];
        cache_max = 12;
    }
    /* Grow the cache if a larger index is requested. */
    else if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)                      /* allocation failed */
            return 0;
        /* Zero the newly allocated part. */
        for (size_t i = cache_size; i < new_size; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Return the already available value if no new computation is
       necessary. */
    if (n <= cache_max)
        return cache[n];

    /* Compute missing terms up to n using the recurrence with the
       linear term (88 n – 1488). */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* Recurrence is valid for i ≥ 13.  For i ≤ 12 the terms are
           already stored. */
        if (i <= 12) {
            continue;
        }

        seq_t pos = 0;
        for (int k = 0; k < 6; ++k)
            pos += cache[i - pos_off[k]] * pos_coef[k];

        seq_t neg = 0;
        for (int k = 0; k < 5; ++k)
            neg += cache[i - neg_off[k]] * neg_coef[k];

        seq_t val = pos - neg;
        val += (seq_t)88 * i;    /* add 88 * n  */
        val -= 1488;             /* subtract 1488  */

        cache[i] = val;
    }

    cache_max = n;
    return cache[n];
}
