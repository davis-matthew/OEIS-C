/*
 * A366557  G.f. A(x) satisfies A(x) = 1 + x + x^3*A(x)^4.
 * https://oeis.org/A366557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊n/3⌋} binomial(3*k+1, n-3*k) * binomial(4*k, k)
 *            / (3*k+1)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 3; ++k)
    {
        idx_t m = n - 3 * k;
        seq_t t1 = binomial(3 * k + 1, m);
        seq_t t2 = binomial(4 * k, k);
        seq_t term = t1 * t2;
        term /= (seq_t)(3 * k + 1);
        sum += term;
    }
    return sum;
}
