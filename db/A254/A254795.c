/*
 * A254795  Numerators of the convergents of the generalized continued
 *          fraction 2 + 1^2/(4 + 3^2/(4 + 5^2/(4 + ... ))).
 * https://oeis.org/A254795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 4*a(n-1) + (2*n-1)^2*a(n-2); a(0)=2, a(1)=9
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)9;

    /* Iterate the recurrence from 2 up to n */
    seq_t prev2 = (seq_t)2;          /* a(0) */
    seq_t prev1 = (seq_t)9;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t im1   = (seq_t)(2*i - 1);          /* (2*i-1) as seq_t */
        seq_t term  = im1 * im1;                /* (2*i-1)^2 */
        seq_t cur   = 4*prev1 + term * prev2;   /* recurrence */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
