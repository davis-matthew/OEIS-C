/*
 * A291223  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - S^3 - S^4.
 * https://oeis.org/A291223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) + a(n-3) - 5*a(n-4) - a(n-5) + 4*a(n-6) - a(n-8)
 *            for n >= 9, with initial values a(0)=0, a(1)=0, a(2)=1, a(3)=1,
 *            a(4)=3, a(5)=5, a(6)=8, a(7)=17, a(8)=25
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 40, u64 n <= 79, u128 n <= 155,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static const seq_t base_values[] = {0,0,1,1,3,5,8,17,25};
static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n < 9)               /* safe initial range: we have the values directly */
        return base_values[(size_t)n];

    size_t need = (size_t)n + 1;
    if (cache_size < need) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* fill the cache up to the required size */
        for (size_t i = cache_size; i < need; ++i) {
            if (i < 9) {
                cache[i] = base_values[i];
            } else {
                seq_t t1 = mul_small(cache[i-2], 4);
                seq_t t2 = cache[i-3];
                seq_t t3 = mul_small(cache[i-4], 5);
                seq_t t4 = cache[i-5];
                seq_t t5 = mul_small(cache[i-6], 4);
                seq_t t6 = cache[i-8];

                seq_t sumPos = t1 + t2 + t5;
                seq_t sumNeg = t3 + t4 + t6;
                cache[i]   = sumPos - sumNeg;
            }
        }
        cache_size = need;
    }
    return cache[(size_t)n];
}
