/*
 * A366591  G.f. A(x) satisfies A(x) = 1 + x^3*(1+x)^2*A(x)^3.
 * https://oeis.org/A366591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*k, n-3*k) *
 *            binomial(3*k, k) / (2*k+1)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 29, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 3) return (n == 0) ? (seq_t)1 : (seq_t)0;
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t r = n - 3 * k;
        idx_t two_k = 2 * k;
        if (r > two_k) continue;
        seq_t t1 = binom(two_k, r);
        seq_t t2 = binom(3 * k, k);
        seq_t term = t1 * t2;
        seq_t denom = (seq_t)(two_k + 1);
        term /= denom;
        sum += term;
    }
    return sum;
}
