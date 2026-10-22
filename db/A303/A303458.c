/*
 * A303458  Number of 4Xn 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4 or 6 horizontally, diagonally or antidiagonally adjacent
 *          elements, with upper left element zero.
 * https://oeis.org/A303458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A303458:
   a(1) = 8
   a(2) = 128
   a(3) = 1696
   a(4) = 22756
   a(5) = 306448
   a(6) = 4129588
   a(7) = 55681352
   a(8) = 750838628
   a(9) = 10125474040
   a(10) = 136549176292
   a(11) = 1841478572144
   a(12) = 24833903347572
   a(13) = 334906650542448
   a(14) = 4516506726074020
   a(15) = 60909019475694136
   a(16) = 821411104147953668
   For n > 16:
   a(n) = 14*a(n-1) + 18*a(n-2) - 338*a(n-3) - 62*a(n-4) + 1192*a(n-5)
          - 648*a(n-6) + 1818*a(n-7) + 1407*a(n-8) - 3144*a(n-9)
          + 3656*a(n-10) - 8832*a(n-11) - 7324*a(n-12) + 8010*a(n-13)
          + 2202*a(n-14) + 3312*a(n-15);

   This function memoizes computed values for efficiency.
*/
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial values (indices 1..16).  init[0] unused. */
    static const seq_t init[17] = {
        0,
        8ULL,
        128ULL,
        1696ULL,
        22756ULL,
        306448ULL,
        4129588ULL,
        55681352ULL,
        750838628ULL,
        10125474040ULL,
        136549176292ULL,
        1841478572144ULL,
        24833903347572ULL,
        334906650542448ULL,
        4516506726074020ULL,
        60909019475694136ULL,
        821411104147953668ULL
    };

    /* Coefficients for the 15‑term recurrence (positive and negative). */
    static const int coeff[15] = {
        14,    /* a(n-1)   */
        18,    /* a(n-2)   */
        -338,  /* a(n-3)   */
        -62,   /* a(n-4)   */
        1192,  /* a(n-5)   */
        -648,  /* a(n-6)   */
        1818,  /* a(n-7)   */
        1407,  /* a(n-8)   */
        -3144, /* a(n-9)   */
        3656,  /* a(n-10)  */
        -8832, /* a(n-11)  */
        -7324, /* a(n-12)  */
        8010,  /* a(n-13)  */
        2202,  /* a(n-14)  */
        3312   /* a(n-15)  */
    };

    /* Return trivial values immediately. */
    if (n <= 16) return init[(idx_t)n];

    /* Lazy memoization cache. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Allocate the base cache (indices 0..16). */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)17 * sizeof(seq_t));
        if (!cache) { /* allocation failed – abort. */
            exit(EXIT_FAILURE);
        }
        cache_size = 17;
        /* copy the known initial terms (cache[1]..cache[16]) */
        for (idx_t i = 1; i < 17; ++i) {
            cache[i] = init[(idx_t)i];
        }
    }

    /* Grow the cache up to the requested n if necessary. */
    if ((idx_t)cache_size <= n) {
        /* Reallocate to a larger size.  Use n+1 to give room for index n. */
        seq_t *new_cache = (seq_t *)realloc(cache, ((size_t)(n + 1)) * sizeof(seq_t));
        if (!new_cache) {
            exit(EXIT_FAILURE);
        }
        cache = new_cache;
        /* Compute new terms from the old last index up to n inclusive. */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            for (int j = 0; j < 15; ++j) {
                idx_t idx = i - 1 - (idx_t)j;  /* index of the (i-j-1)-th previous term */
                seq_t val = cache[idx];
                int c = coeff[j];
                if (c >= 0) {
                    sum_pos += val * (seq_t)c;
                } else {
                    sum_neg += val * (seq_t)(-c);
                }
            }
            cache[i] = sum_pos - sum_neg;
        }
        cache_size = (size_t)(n + 1);
    }

    return cache[n];
}
