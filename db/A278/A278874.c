/*
 * A278874  Number of tilings of a 2 X n rectangle using pentominoes of any
 *          shape and monominoes.
 * https://oeis.org/A278874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating
 * function
 *   G(x) = -(x^10 + x^8 - x^6 - 6*x^5 - x^4 - 5*x^3 + 1)
 *          / (x^15 + x^13 - 2*x^11 - 11*x^10 - 2*x^9 - 10*x^8
 *             + x^7 + 9*x^6 + 12*x^5 + 8*x^4 + 11*x^3 + x - 1)
 *
 * The coefficients of the denominator are
 *   d[0] = -1, d[1] =  1, d[2] =  0, d[3] = 11, d[4] =  8, d[5] = 12,
 *   d[6] =  9, d[7] =  1, d[8] = -10, d[9] = -2, d[10] = -11, d[11] = -2,
 *   d[12] =  0, d[13] =  1, d[14] =  0, d[15] =  1
 *
 * The recurrence is
 *   a(n) = Σ_{k=1}^{15} d[k] * a(n-k) - p(n)
 *
 * where
 *   p(0) = -1, p(3) = 5, p(4) = 1, p(5) = 6, p(6) = 1,
 *   p(8) = -1, p(10) = -1, and p(n) = 0 otherwise.
 *
 * Initial terms are supplied automatically by the recurrence.
 */

#include <stdlib.h>

static const int d[16] = { -1,  1,  0, 11,  8, 12,
                            9,   1, -10, -2, -11, -2,
                            0,   1,  0,  1 };

static inline int p_value(idx_t n)
{
    switch (n) {
    case 0:  return -1;
    case 3:  return  5;
    case 4:  return  1;
    case 5:  return  6;
    case 6:  return  1;
    case 8:  return -1;
    case 10: return -1;
    default: return  0;
    }
}

/* Dynamic cache for computed values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Ensure the cache can hold indices up to idx */
static void ensure_cache(idx_t idx)
{
    size_t needed = (size_t)(idx + 1);
    if (needed <= cache_cap)
        return;

    size_t new_cap = needed * 2;
    if (new_cap < 64) new_cap = 64; /* minimal growth */

    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) abort(); /* out of memory */
    cache = new_buf;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n >= cache_len) {
        ensure_cache(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t pos_sum = 0;
            seq_t neg_sum = 0;
            for (int k = 1; k <= 15; ++k) {
                if (i >= (idx_t)k) {
                    seq_t prod = cache[i - k];
                    prod *= (seq_t)(d[k] >= 0 ? d[k] : -d[k]); /* positive go to prod */
                    if (d[k] >= 0)
                        pos_sum += prod;
                    else
                        neg_sum += prod;
                }
            }
            seq_t val = pos_sum;
            val -= neg_sum;                    /* pos_sum - neg_sum  (guaranteed >=0) */

            int pi = p_value(i);
            if (pi > 0)
                val -= (seq_t)pi;
            else if (pi < 0)
                val += (seq_t)(-pi);          /* subtract negative => add */

            cache[i] = val;
        }
        cache_len = (size_t)(n + 1);
    }
    return cache[n];
}
