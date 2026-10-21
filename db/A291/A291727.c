/*
 * A291727  p-INVERT of (1,0,1,0,0,0,0,...), where p(S) = (1 - S)^4.
 * https://oeis.org/A291727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 8*a(n-3) - 13*a(n-4) + 12*a(n-5) -
 *            10*a(n-6) + 12*a(n-7) - 6*a(n-8) + 4*a(n-9) - 4*a(n-10) -
 *            a(n-12) (initial values: a(0)=4, a(1)=10, a(2)=24, a(3)=55,
 *            a(4)=116, a(5)=234, a(6)=460, a(7)=879, a(8)=1640, a(9)=3006,
 *            a(10)=5424, a(11)=9650, a(12)=16964)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 37, u64 n <= 89, u128 n <= 199,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* ensure cache exists and is large enough */
    if (cache_len == 0 || (size_t)n >= cache_len) {
        /* determine target length */
        size_t target_len = (size_t)(n >= 12 ? (idx_t)n + 1 : 13);

        /* allocate or grow cache */
        if (cache_len == 0) {
            cache = (seq_t *)malloc(target_len * sizeof(seq_t));
            if (!cache) return 0;   /* allocation failure: return zero */
        } else {
            cache = (seq_t *)realloc(cache, target_len * sizeof(seq_t));
            if (!cache) return 0;
        }

        /* initialise first 13 terms if this is the first call */
        if (cache_len == 0) {
            const seq_t init_vals[13] = {
                4ULL, 10ULL, 24ULL, 55ULL, 116ULL, 234ULL, 460ULL,
                879ULL, 1640ULL, 3006ULL, 5424ULL, 9650ULL, 16964ULL
            };
            for (size_t i = 0; i < 13; ++i)
                cache[i] = init_vals[i];
        }

        /* compute new terms if needed */
        size_t start = (cache_len > 13) ? cache_len : 13;
        for (size_t i = start; i <= (size_t)n; ++i) {
            /* compute positive and negative sums separately to avoid
               intermediate underflow/overflow */
            seq_t pos = 0, neg = 0;
            pos += 4 * cache[i - 1];
            pos += 8 * cache[i - 3];
            pos += 12 * cache[i - 5];
            pos += 12 * cache[i - 7];
            pos += 4 * cache[i - 9];

            neg += 6 * cache[i - 2];
            neg += 13 * cache[i - 4];
            neg += 10 * cache[i - 6];
            neg += 6 * cache[i - 8];
            neg += 4 * cache[i - 10];
            neg += cache[i - 12];

            cache[i] = pos - neg;   /* pos >= neg for all n >= 13 */
        }

        /* update cache length */
        cache_len = target_len;
    }

    return cache[(size_t)n];
}
