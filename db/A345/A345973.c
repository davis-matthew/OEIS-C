/*
 * A345973  G.f.: x + x^2 / Product_{n>=1} (1 - a(n)*x^n).
 * https://oeis.org/A345973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 29, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence):
   a(1)=a(2)=a(3)=1,
   a(n+2) = (1/n) * Σ_{k=1}^{n}  ( Σ_{d|k} d * a(d)^(k/d) ) * a(n-k+2)     (n≥1)
   The inner sum is denoted b(k) = Σ_{d|k} d * a(d)^(k/d).
   Thus a(m) = (1/(m-2)) * Σ_{k=1}^{m-2} b(k) * a(m-k)   (m≥4)
*/

#include <stdlib.h>
#include "../utils/big_pow.h"

static seq_t *a_cache = NULL;   /* a(i) values, 1‑based indexing */
static seq_t *b_cache = NULL;   /* b(i) values, 1‑based indexing */
static idx_t cache_size = 0;    /* allocated capacity (≥ current max index + 1) */

seq_t a(idx_t n)
{
    /* Ensure capacity up to n */
    if (n + 1 > cache_size)
    {
        idx_t new_size = cache_size ? cache_size : 16;
        while (new_size <= n + 1)
            new_size <<= 1;          /* power‑of‑two growth */
        a_cache = (seq_t *)realloc(a_cache, sizeof(seq_t) * new_size);
        b_cache = (seq_t *)realloc(b_cache, sizeof(seq_t) * new_size);
        cache_size = new_size;
    }

    /* Lazy initialization of known values */
    if (!a_cache[1])          /* not yet initialized */
    {
        a_cache[1] = a_cache[2] = a_cache[3] = (seq_t)1;

        /* Compute b(1), b(2), b(3) */
        b_cache[1] = (seq_t)1;   /* d=1: 1 * a(1)^1 = 1 */

        /* b(2) = 1*a(1)^2 + 2*a(2)^1 */
        b_cache[2] = (seq_t)1 * big_pow(a_cache[1], (seq_t)2) +
                     (seq_t)2 * big_pow(a_cache[2], (seq_t)1);

        /* b(3) = 1*a(1)^3 + 3*a(3)^1 */
        b_cache[3] = (seq_t)1 * big_pow(a_cache[1], (seq_t)3) +
                     (seq_t)3 * big_pow(a_cache[3], (seq_t)1);
    }

    /* Compute needed values up to n */
    for (idx_t m = 4; m <= n; ++m)
    {
        if (a_cache[m])          /* already computed */
            continue;

        /* a(m) via convolution with b(k) */
        seq_t sum = 0;
        for (idx_t k = 1; k <= m - 2; ++k)
            sum += b_cache[k] * a_cache[m - k];

        a_cache[m] = sum / (seq_t)(m - 2);

        /* Compute b(m) = Σ_{d|m} d * a(d)^(m/d) */
        seq_t bm = 0;
        for (idx_t d = 1; d * d <= m; ++d)
        {
            if (m % d == 0)
            {
                /* divisor d */
                bm += (seq_t)d * big_pow(a_cache[d], (seq_t)(m / d));

                /* paired divisor m/d, avoid double counting if square */
                idx_t d2 = m / d;
                if (d2 != d)
                    bm += (seq_t)d2 * big_pow(a_cache[d2], (seq_t)(m / d2));
            }
        }
        b_cache[m] = bm;
    }

    return a_cache[n];
}
