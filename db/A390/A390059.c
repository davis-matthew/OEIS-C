/*
 * A390059  E.g.f. A(x) satisfies A(x) = exp( x * (1+x^3) * A(x)^2 ).
 * https://oeis.org/A390059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * sum_{k=0..⌊n/4⌋} (2*(n-3k)+1)^(n-3k-1) *
 *            binomial(n-3k, k) / (n-3k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, mul_small, pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/mul_small.h"
#include "../utils/seq_mul.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;                   /* a(0) = 1 */
    }

    seq_t sum = 0;                           /* running sum of terms */
    idx_t maxk = n / 4;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - 3 * k;                /* m = n - 3k, always >= 0 */

        /* Compute n! / m! as product of integers (m+1 .. n) */
        seq_t prod = 1;
        for (idx_t t = m + 1; t <= n; ++t) {
            prod = mul_small(prod, (unsigned int)t);
        }

        /* (2*m + 1)^(m-1) */
        seq_t base = 2 * m + 1;
        seq_t pow_term = pow_seq(base, (int)(m - 1));

        /* binomial coefficient */
        seq_t bin = binomial(m, k);

        /* term = prod * pow_term * bin */
        seq_t term = seq_mul(prod, pow_term);
        term = seq_mul(term, bin);

        sum += term;          /* seq_t supports the + operator */
    }

    return sum;
}
