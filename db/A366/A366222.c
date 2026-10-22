/*
 * A366222  G.f. A(x) satisfies A(x) = 1 + x*(1 + x)^4*A(x)^3.
 * https://oeis.org/A366222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*k, n-k) * binomial(3*k,
 *            k)/(2*k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t a1 = 4 * k;
        idx_t b1 = n - k;
        if (b1 > a1) continue;                /* binomial(4k, n-k) = 0 when n-k > 4k */
        seq_t term = binom(a1, b1);           /* C(4k, n-k)   */
        term = term * binom(3 * k, k);       /* *C(3k, k)    */
        term = term / (2 * k + 1);           /* divide by (2k+1) */
        sum += term;
    }
    return sum;
}
