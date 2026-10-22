/*
 * A386958  a(n) = Sum_{k=0..n} 8^k * binomial(k-2/3,k) *
 *          binomial(2*n+1/3,n-k).
 * https://oeis.org/A386958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A386958
   Formula: a(n) = Σ_{k=0..n} 3^k * (∏_{t=0}^{k-1} (3t+1)) / k! * binom(2n−k, n−k)
   Offset: 0
   Remark: Uses the 'binomial' routine from ../utils/binomial.h. */

#include "../utils/binomial.h"

/* Return a(n).  The result is exact as long as it fits in type seq_t. */
seq_t a(idx_t n)
{
    seq_t sum = 0;        /* accumulator for the sum */
    seq_t g   = 1;        /* g(k) = 3^k * ∏_{t=0}^{k-1} (3t+1) / k!  */
    for (idx_t k = 0; ; ++k)
    {
        /* binomial(2n−k, n−k) is always an integer. */
        seq_t bin = binomial(2 * n - k, n - k);
        sum += g * bin;

        if (k == n)
            break;               /* finished the sum */

        /* Update g for the next k:
           g(k+1) = g(k) * 3 * (3k+1) / (k+1)  */
        seq_t mul = 3 * (3 * k + 1);           /* 3 * (3k+1) */
        g = (g * mul) / (seq_t)(k + 1);        /* exact division */
    }
    return sum;
}
