/*
 * A386862  a(n) = Sum_{k=0..n} 2^(n-k) * binomial(2*n+1,k) *
 *          binomial(2*n-k-1,n-k).
 * https://oeis.org/A386862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k) * binomial(2*n+1,k) *
 *            binomial(2*n-k-1,n-k) (OEIS A386862)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1   (binomial(-1,0) is not defined by the
       standard binomial implementation in the utilities). */
    if (n == 0)
        return 1;

    seq_t sum  = 0;
    seq_t pow2 = pow_uint(2, n);                /* 2^n */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = pow2;
        term *= binomial(2*n + 1, k);
        term *= binomial(2*n - k - 1, n - k);
        sum += term;

        /* Prepare 2^(n-(k+1)) for the next iteration */
        if (k < n)
            pow2 /= 2;                         /* exact division */
    }

    return sum;
}
