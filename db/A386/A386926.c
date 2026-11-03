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
 * Validated: standard, pass, 77 terms through n = 9766 (2026-11-01)
 * Helpers:   gcd, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/num_divisors.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

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
            seq_t d_k = (seq_t)num_divisors((uint64_t)k);
            seq_t d_k_plus1 = (seq_t)num_divisors((uint64_t)(k + 1));

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
