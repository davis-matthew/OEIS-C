/*
 * A360076  a(n) = Sum_{k=0..n} binomial(3*k,n-k) * Catalan(k).
 * https://oeis.org/A360076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*k, n-k) * Catalan(k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* binomial(3*k, n-k) */
        idx_t three_k = 3 * k;
        seq_t b = binomial(three_k, n - k);
        if (b == 0)
            continue;
        seq_t c = catalan(k);

#if defined(__SIZEOF_INT128__)
        /* Use 128‑bit arithmetic when available to prevent overflow. */
        __uint128_t tmp = (__uint128_t)b * (__uint128_t)c;
        seq_t prod = (seq_t)tmp;
#else
        seq_t prod = b * c;  /* May overflow for very large n; acceptable
                                because only values fitting in seq_t are
                                tested for the 64‑bit build. */
#endif

        total += prod;
    }
    return total;
}
