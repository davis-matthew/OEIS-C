/*
 * A255242  Calculate the aliquot parts of a number n and take their sum. Then
 *          repeat the process calculating the aliquot parts of all the
 *          previous aliquot parts and add their sum to the previous one.
 *          Repeat the process until the sum to be added is zero. Sequence
 *          lists these sums.
 * https://oeis.org/A255242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>=2, a(n)=σ(n)-n + Σ_{d|n, d<n} a(d)
 * Range:     u8 n <= 47, u16 n <= 2159, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* σ(n) is the sum of all divisors of n.  The recurrence uses the sums
   of proper divisors and recursively the sequence values for all
   smaller divisors.  The implementation computes the sequence
   incrementally up to the requested index using a divisor
   enumeration loop for each new term. */

#include <stdlib.h>
#include <math.h>

static seq_t *cache = NULL;       /* cache[0] unused; cache[1] = 0 */
static size_t cache_len = 0;      /* number of cached entries (index 0 unused) */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (size_t i = cache_len; i < new_len; ++i)
            cache[i] = 0;

        size_t start = (cache_len > 1 ? cache_len : 2);
        for (size_t m = start; m <= (size_t)n; ++m) {
            seq_t sum_d = 0, sum_a = 0;
            size_t sq = (size_t)sqrt((double)m);
            for (size_t d = 1; d <= sq; ++d) {
                if (m % d == 0) {
                    size_t d1 = d;
                    size_t d2 = m / d;
                    if (d1 != m) {
                        sum_d += (seq_t)d1;
                        sum_a += cache[d1];
                    }
                    if (d2 != d1 && d2 != m) {
                        sum_d += (seq_t)d2;
                        sum_a += cache[d2];
                    }
                }
            }
            cache[m] = sum_d + sum_a;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
