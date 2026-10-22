/*
 * A369436  Number of 2-Motzkin meanders with catastrophes of length n.
 * https://oeis.org/A369436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (12*n-8)*a(n-1) + (70-53*n)*a(n-2) + (107*n-210)*a(n-3)
 *            + (268-101*n)*a(n-4) + (36*n-126)*a(n-5)   all divided by n
 * Initial values: a(0)=1, a(1)=3, a(2)=10, a(3)=36, a(4)=136
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

static const seq_t base_vals[5] = {1,3,10,36,136};

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    if (n < 5) {
        return base_vals[n];
    }

    if (cache_len == 0) {
        cache_cap = n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 5; ++i) cache[i] = base_vals[i];
        cache_len = 5;
    }
    if (n >= cache_len) {
        /* Grow cache if needed */
        if ((size_t)n + 1 > cache_cap) {
            cache_cap = (size_t)n + 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        for (size_t i = cache_len; i <= n; ++i) {
            int64_t coeff1 = 12 * (int64_t)i - 8;
            int64_t coeff2 = 70 - 53 * (int64_t)i;
            int64_t coeff3 = 107 * (int64_t)i - 210;
            int64_t coeff4 = 268 - 101 * (int64_t)i;
            int64_t coeff5 = 36 * (int64_t)i - 126;

            seq_t pos = 0;
            seq_t neg = 0;

            /* term1 */
            if (coeff1 >= 0)
                pos += mul_small(cache[i-1], (unsigned int)coeff1);
            else
                neg += mul_small(cache[i-1], (unsigned int)(-coeff1));

            /* term2 */
            if (coeff2 >= 0)
                pos += mul_small(cache[i-2], (unsigned int)coeff2);
            else
                neg += mul_small(cache[i-2], (unsigned int)(-coeff2));

            /* term3 */
            if (coeff3 >= 0)
                pos += mul_small(cache[i-3], (unsigned int)coeff3);
            else
                neg += mul_small(cache[i-3], (unsigned int)(-coeff3));

            /* term4 */
            if (coeff4 >= 0)
                pos += mul_small(cache[i-4], (unsigned int)coeff4);
            else
                neg += mul_small(cache[i-4], (unsigned int)(-coeff4));

            /* term5 */
            if (coeff5 >= 0)
                pos += mul_small(cache[i-5], (unsigned int)coeff5);
            else
                neg += mul_small(cache[i-5], (unsigned int)(-coeff5));

            seq_t numerator = pos - neg;   /* pos >= neg for all valid n */
            cache[i] = numerator / (seq_t)i;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
