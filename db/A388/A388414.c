/*
 * A388414  a(n) = Sum_{k=0..n} 4^(n-k) * binomial(n,k) * binomial(n+4,k).
 * https://oeis.org/A388414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} 4^(n-k) * binomial(n,k) * binomial(n+4,k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   binomial, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = pow4(n - k);               /* 4^(n-k) */
        term *= binomial(n, k);
        term *= binomial(n + 4, k);
        sum += term;
    }
    return sum;
}
