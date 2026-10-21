/*
 * A266354  Expansion of b(2)*b(6)*b(10)/(1 - x - x^2 - x^4 - x^5 + x^11 +
 *          x^12 + x^14), where b(k) = (1-x^k)/(1-x).
 * https://oeis.org/A266354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 34, u64 n <= 72, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A266354.
   G.f. = b(2)*b(6)*b(10) /
          (1 - x - x^2 - x^4 - x^5 + x^11 + x^12 + x^14)
   with b(k) = (1-x^k)/(1-x).

   The denominator yields the linear recurrence
   a(n) = r(n) + 4*a(n-1) +  a(n-3) + 3*a(n-4)
        + 2*a(n-7) + 1*a(n-8) + 2*a(n-12) + 4*a(n-15) + 1*a(n-17)
          - 5*a(n-2) - 3*a(n-5) - 1*a(n-11) - 3*a(n-14) - 3*a(n-16),
   where r(n) =  1  (n=0)
                  -1 (n=2)
                  -1 (n=6)
                   1 (n=8)
                  -1 (n=10)
                   1 (n=12)
                   1 (n=16)
                  -1 (n=18)
                0 otherwise.
   This is implemented using unsigned arithmetic only (no negative
   intermediate values).  The function is memoised for efficiency.   */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Coefficients of the recurrence (c[0] = 1).  Positive values are
   subtracted, negative values are added. */
static const int8_t ccoeff[18] = {
    1,   /* c[0] */
   -4,   /* c[1] */
    5,   /* c[2] */
   -1,   /* c[3] */
   -3,   /* c[4] */
    3,   /* c[5] */
    0,   /* c[6] */
   -2,   /* c[7] */
    1,   /* c[8] */
    0,   /* c[9] */
    0,   /* c[10] */
    1,   /* c[11] */
   -2,   /* c[12] */
    0,   /* c[13] */
    3,   /* c[14] */
   -4,   /* c[15] */
    3,   /* c[16] */
   -1    /* c[17] */
};

/* Remainder polynomial Q(x) = 1 - x^2 - x^6 + x^8 - x^10 + x^12 + x^16 - x^18.
   rcoeff[n] holds the coefficient of x^n in Q(x).  n <= 18. */
static const int8_t rcoeff[19] = {
    1,   /* 0 */
    0,   /* 1 */
   -1,   /* 2 */
    0,   /* 3 */
    0,   /* 4 */
    0,   /* 5 */
   -1,   /* 6 */
    0,   /* 7 */
    1,   /* 8 */
    0,   /* 9 */
   -1,   /* 10 */
    0,   /* 11 */
    1,   /* 12 */
    0,   /* 13 */
    0,   /* 14 */
    0,   /* 15 */
    1,   /* 16 */
    0,   /* 17 */
   -1    /* 18 */
};

seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t sumPos = 0;
        seq_t sumNeg = 0;

        /* Remainder coefficient */
        if (i <= 18) {
            int8_t r = rcoeff[i];
            if (r > 0)
                sumPos += (seq_t)r;
            else if (r < 0)
                sumNeg += (seq_t)(-r);
        }

        /* Recursive part */
        for (int k = 1; k <= 17; ++k) {
            if (i < (size_t)k) continue;        /* term is zero for negative index */

            int8_t ck = ccoeff[k];
            if (ck < 0) {
                /* add (-ck) * a[i-k] */
                sumPos += cache[i - k] * (seq_t)(-ck);
            } else if (ck > 0) {
                /* subtract ck * a[i-k] */
                sumNeg += cache[i - k] * (seq_t)ck;
            }
        }

        /* Final value: sumPos - sumNeg (guaranteed non‑negative) */
        cache[i] = sumPos - sumNeg;
    }
    cache_len = n + 1;
    return cache[n];
}
