/*
 * A366589  G.f. A(x) satisfies A(x) = 1 + x^4*(1+x)*A(x)^2.
 * https://oeis.org/A366589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binomial(k, n-4*k) * Catalan(k)
 * Range:     u8 n <= 21, u16 n <= 34, u32 n <= 60, u64 n <= 109,
 *            u128 n <= 206, big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - 4 * k;
        /* binomial(k, m) is zero if m>k; guard to avoid unnecessary work */
        if (m > k) continue;
        seq_t comb = binomial(k, m);
        if (!comb) continue;
        seq_t cat = catalan(k);
        sum += comb * cat;
    }
    return sum;
}
