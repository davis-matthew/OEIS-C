/*
 * A366592  G.f. A(x) satisfies A(x) = 1 + x^4*(1+x)^2*A(x)^3.
 * https://oeis.org/A366592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(2k, n-4k) * binomial(3k,
 *            k) / (2k+1)
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 37, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n / 4; ++k) {
        idx_t nminus = n - 4 * k;
        idx_t two_k  = 2 * k;
        if (nminus > two_k) {
            continue; /* binomial(two_k, nminus) would be zero */
        }
        seq_t term = binomial(two_k, nminus);
        term = term * binomial(3 * k, k);
        term = term / ((seq_t)(2 * k + 1));
        total += term;
    }
    return total;
}
