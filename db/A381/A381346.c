/*
 * A381346  Expansion of e.g.f. 1/( 1 - sinh(sqrt(2)*x) / sqrt(2) ).
 * https://oeis.org/A381346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} 2^k * binomial(n, 2*k+1) *
 *            a(n-2*k-1) with a(0)=1, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   binomial, ensure_cache, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"
#include "../utils/ensure_cache.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    ensure_cache(n, &cache, &cache_cap);

    if (cache[n] != 0)
        return cache[n];

    seq_t res;
    if (n == 0 || n == 1)
        res = 1;
    else {
        res = 0;
        idx_t kmax = (n - 1) / 2;
        for (idx_t k = 0; k <= kmax; ++k) {
            seq_t term = binomial(n, 2 * k + 1);
            term = seq_mul(term, pow2(k));
            term = seq_mul(term, a(n - 2 * k - 1));
            res += term;
        }
    }

    cache[n] = res;
    return res;
}
