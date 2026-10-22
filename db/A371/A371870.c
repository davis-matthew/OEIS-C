/*
 * A371870  a(n) = Sum_{k=0..floor(n/2)} binomial(2*n-k-1,n-2*k).
 * https://oeis.org/A371870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(2*n-k-1, n-2*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a multiplicative recurrence for the binomial
   terms that avoids recomputing a factorial each time. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    idx_t m = n / 2;               /* floor(n/2) */
    seq_t sum = 0;

    /* Initial term for k = 0 : binomial(2*n-1, n) */
    idx_t B = n;                   /* = n - 2*k for k=0 */
    idx_t A = 2 * n - 1;           /* = 2*n - k - 1 for k=0 */
    seq_t term = binomial(A, B);
    sum += term;

    /* Iterate k = 1 .. m using the recurrence
       T_{k+1} = T_k * (B*(B-1)) / (A*(A-B+1))   where
       B = n - 2*k  and  A = 2*n - k - 1.        */
    for (idx_t k = 1; k <= m; ++k)
    {
        seq_t num = (seq_t)B * (seq_t)(B - 1);
        seq_t den = (seq_t)A * (seq_t)(A - B + 1);
        term = (term * num) / den;
        sum += term;

        B -= 2;
        A -= 1;
    }

    return sum;
}
