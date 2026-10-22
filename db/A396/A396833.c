/*
 * A396833  The constant coefficient of (x + y + y^-1 + x^-1 + x^-1*y^-1)^n.
 * https://oeis.org/A396833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} binomial(n,k) * binomial(2*k,k) *
 *            binomial(k,n-2*k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 60,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        seq_t t1 = binomial(n, k);
        seq_t t2 = binomial(2 * k, k);
        seq_t t3 = binomial(k, n - 2 * k);
        result += t1 * t2 * t3;
    }
    return result;
}
