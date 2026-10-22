/*
 * A301493  Number of nX3 0..1 arrays with every element equal to 0, 1, 3, 4,
 *          5 or 6 horizontally, vertically or antidiagonally adjacent
 *          elements, with upper left element zero.
 * https://oeis.org/A301493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A301493: Number of n×3 0..1 arrays with every element equal to 0, 1, 3, 4, 5 or 6 horizontally, vertically or antidiagonally adjacent elements, with upper left element zero.
   Sequence starts at n = 1.
   The sequence satisfies the (empirical) linear recurrence
     a(n) = a(n-1) + 5 a(n-2) + 5 a(n-3) + 12 a(n-4) - 13 a(n-5) - 66 a(n-6)
            - 64 a(n-7) - 34 a(n-8) + 72 a(n-9) + 226 a(n-10) + 194 a(n-11)
            - 29 a(n-12) - 203 a(n-13) - 99 a(n-14) + 87 a(n-15) + 78 a(n-16)
            - 9 a(n-17) - 24 a(n-18) - 6 a(n-19)    for n>23,
   with the initial values below. */

#include <stdlib.h>
#include <stdint.h>

/* Initial terms: a(0) unused; a(1) … a(24) as given in the OEIS entry */
static const seq_t init_values[25] = {
    0ULL,                     /* a(0) */
    3ULL, 2ULL, 5ULL, 15ULL, 45ULL, 156ULL, 479ULL, 1455ULL, 4493ULL,
    14037ULL, 43495ULL, 134587ULL, 417047ULL, 1293362ULL, 4008799ULL,
    12423540ULL, 38505876ULL, 119353345ULL, 369934949ULL, 1146596091ULL,
    3553853570ULL, 11015153343ULL, 34141324269ULL, 105820474738ULL
};

/* Recurrence coefficients and offsets (indices n-1 … n-19) */
static const int16_t coeffs[19] = {
    1,   5,   5,   12, -13, -66, -64, -34,
    72, 226, 194, -29, -203, -99, 87, 78,
   -9, -24, -6
};
static const int8_t offsets[19] = {
    1, 2, 3, 4, 5, 6, 7, 8,
    9,10,11,12,13,14,15,16,
   17,18,19
};

seq_t a(idx_t n)
{
    if (n <= 0) return 0;          /* offset 1: a(1) is first term */

    /* Lazy static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;    /* allocated size minus one */
    static size_t cache_next = 0;   /* first index not yet computed */

    /* Initialise cache on first call */
    if (!cache) {
        cache_cap = 32;             /* start with capacity 32 indices (0..32) */
        cache     = (seq_t*)calloc(cache_cap + 1, sizeof(seq_t));
        if (!cache) return 0;       /* allocation failure */
        for (size_t i = 0; i <= 24; ++i)
            cache[i] = init_values[i];
        cache_next = 25;            /* next index to be filled */
    }

    /* Expand cache if n is outside current capacity */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n > new_cap) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0;         /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    while (cache_next <= (size_t)n) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;

        for (int i = 0; i < 19; ++i) {
            size_t idx = cache_next - offsets[i];
            seq_t val  = cache[idx];
            int16_t coeff = coeffs[i];
            seq_t mul  = val * (seq_t)(coeff >= 0 ? coeff : -coeff);
            if (coeff >= 0)
                pos_sum += mul;
            else
                neg_sum += mul;
        }
        cache[cache_next] = pos_sum - neg_sum;
        ++cache_next;
    }

    return cache[(size_t)n];
}
