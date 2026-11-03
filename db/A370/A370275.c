/*
 * A370275  Coefficient of x^n in the expansion of 1/( (1-x)^2 * (1-x^3)^2
 *          )^n.
 * https://oeis.org/A370275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/3⌋} binomial(2*n+k-1, k) * binomial(3*n-3*k-1,
 *            n-3*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t total = 0;
    idx_t maxk = n / 3;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t a1 = 2 * n + k - 1;
        idx_t b1 = k;
        idx_t a2 = 3 * n - 3 * k - 1;
        idx_t b2 = n - 3 * k;

        seq_t term1 = binomial(a1, b1);
        seq_t term2 = binomial(a2, b2);
        total += term1 * term2;
    }

    return total;
}
