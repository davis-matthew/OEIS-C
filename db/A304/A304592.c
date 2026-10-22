/*
 * A304592  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3, 4
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A304592
   Number of n×3 0..1 arrays with every element unequal to 1, 2, 3, 4 or 6
   king‑move adjacent elements, with upper left element zero.
   Offset: 1   (so a(1) is the first term listed below)

   Recurrence (for n > 16):
     a(n) = 3*a(n-1) + 9*a(n-2) + 13*a(n-3) - 38*a(n-4) - 46*a(n-5)
            + 28*a(n-6) - 45*a(n-7) - 54*a(n-8) - 34*a(n-9)
            + 8*a(n-10) + 26*a(n-11) - 37*a(n-12) - 7*a(n-13)
            - 3*a(n-14) + 2*a(n-15)
   Initial values (n = 1 .. 16):
     1, 23, 82, 456, 2207, 10949, 54357, 269732, 1337127,
     6635686, 32914816, 163277978, 810008547, 4018201206,
     19933310071, 98884549774
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial 16 terms */
    static const seq_t init[16] = {
        (seq_t)1U,
        (seq_t)23U,
        (seq_t)82U,
        (seq_t)456U,
        (seq_t)2207U,
        (seq_t)10949U,
        (seq_t)54357U,
        (seq_t)269732U,
        (seq_t)1337127U,
        (seq_t)6635686U,
        (seq_t)32914816U,
        (seq_t)163277978U,
        (seq_t)810008547U,
        (seq_t)4018201206U,
        (seq_t)19933310071U,
        (seq_t)98884549774U
    };

    /* Create cache for computed values on demand */
    static seq_t *cache       = NULL;
    static size_t cache_len   = 0;    /* number of terms currently stored */
    static size_t cache_cap   = 0;    /* allocated capacity */

    /* Ensure we have at least 16 terms known */
    if (n <= 16u)               /* fast path for the requested term itself */
        return init[n - 1];

    /* Allocate initial cache if needed */
    if (cache_len < 16) {
        if (!cache) {
            cache_cap = 16;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_cap < 16) {
            cache_cap = 16;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        for (size_t i = cache_len; i < 16; ++i)
            cache[i] = init[i];
        cache_len = 16;
    }

    /* Grow cache to size n if required */
    if (cache_len < (size_t)n) {
        if (n > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < (size_t)n)
                new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Coefficients of the recurrence (a(n‑1) .. a(n‑15)) */
        static const long long coeffs[15] = {
            3LL, 9LL, 13LL, -38LL, -46LL, 28LL, -45LL, -54LL, -34LL,
             8LL, 26LL, -37LL, -7LL, -3LL, 2LL
        };

        /* Compute missing terms up to n, using the recurrence */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t sum_pos = 0;   /* sum of positive coefficient terms */
            seq_t sum_neg = 0;   /* sum of negative coefficient terms */

            for (int k = 0; k < 15; ++k) {
                size_t idx = (size_t)(i - 2 - k);  /* 0‑based index of a(i‑(k+1)) */
                seq_t term = cache[idx];
                long long c   = coeffs[k];
                if (c > 0)
                    sum_pos += term * (seq_t)c;
                else
                    sum_neg += term * (seq_t)(-c);
            }
            cache[i - 1] = sum_pos - sum_neg;
        }
        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
