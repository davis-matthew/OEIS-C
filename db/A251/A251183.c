/*
 * A251183  a(n) = Sum_{k=0..n} binomial(n,k) * (-1)^(n-k) * (2^k + 1)^k.
 * https://oeis.org/A251183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * (-1)^(n-k) * (2^k + 1)^k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum_pos = 0;         /* accumulate terms with even sign */
    seq_t sum_neg = 0;         /* accumulate terms with odd sign */

    for (idx_t k = 0; k <= n; ++k) {
        /* binomial coefficient C(n,k) */
        seq_t bin = binomial(n, k);

        /* compute base = 2^k + 1 */
        seq_t base = pow2(k);
        base += 1;

        /* term = base^k */
        seq_t term = big_pow(base, (seq_t)k);

        /* term = bin * term */
        term *= bin;

        /* decide sign */
        if ( ((n - k) & 1) == 0 )
            sum_pos += term;   /* even exponent of -1 : positive */
        else
            sum_neg += term;   /* odd exponent of -1 : negative */
    }

    /* final result = sum_pos - sum_neg (positive for all n) */
    return sum_pos - sum_neg;
}
