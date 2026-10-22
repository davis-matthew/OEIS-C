/*
 * A372413  Coefficient of x^n in the expansion of ( (1-x+x^3) / (1-x) )^n.
 * https://oeis.org/A372413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n,k) *
 *            binomial(n-2*k-1,n-3*k)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special case n == 0: a(0) = 1 */
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    idx_t max_k = n / 3;   /* floor(n/3) */

    for (idx_t k = 0; k <= max_k; ++k)
    {
        seq_t b1 = binomial(n, k);
        idx_t a1 = n - 2 * k - 1;       /* n - 2k - 1  (always non-negative here) */
        idx_t b2_index = n - 3 * k;     /* n - 3k       */
        seq_t b2 = binomial(a1, b2_index);
        sum += b1 * b2;
    }

    return sum;
}
