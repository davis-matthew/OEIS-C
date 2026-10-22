/*
 * A370282  Coefficient of x^n in the expansion of 1/( (1-x)^3 - x )^n.
 * https://oeis.org/A370282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k-1,k) * binomial(4*n+2*k-1,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term1 = binomial(n + k - 1, k);
        seq_t term2 = binomial(4 * n + 2 * k - 1, n - k);
        sum += seq_mul(term1, term2);
    }
    return sum;
}
