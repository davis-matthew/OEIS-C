/*
 * A371608  G.f. satisfies A(x) = ( 1 + x * (1 + x*A(x)^3) )^2.
 * https://oeis.org/A371608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(6*(n-k)+2, k) * binomial(k,
 *            n-k)/(3*(n-k)+1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t nmk = n - k;
        seq_t term = binom(6 * nmk + 2, k);
        term *= binom(k, nmk);
        term /= (seq_t)(3 * nmk + 1);
        sum += term;
    }
    return sum;
}
