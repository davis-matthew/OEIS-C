/*
 * A386926  a(n) = denominator(Sum_{k=1..n} d(k+1)/d(k)), where d is the
 *          number of divisors function.
 * https://oeis.org/A386926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of sum_{k=1..n} d(k+1)/d(k)
 * Range:     u8 n <= 8, u16 n <= 37, u32 n <= 1449, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, gcd, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/gcd.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t numerator = 0;
    static seq_t denominator = 1;
    static seq_t *den_cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    if (n < 1) return 0;                /* undefined for n=0 */

    if (cache_len < n) {
        /* grow cache if necessary */
        if (cache_cap < n) {
            den_cache = seq_realloc(den_cache, cache_cap, n);
            cache_cap = n;
        }
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            seq_t d_k = (seq_t)divisor_count_u64((uint64_t)k);
            seq_t d_k_plus1 = (seq_t)divisor_count_u64((uint64_t)(k + 1));

            seq_t new_num = numerator * d_k + denominator * d_k_plus1;
            seq_t new_den = denominator * d_k;
            seq_t g = gcd(new_num, new_den);

            numerator = new_num / g;
            denominator = new_den / g;

            den_cache[cache_len] = denominator;
            ++cache_len;
        }
    }
    return den_cache[n - 1];
}
