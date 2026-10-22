/*
 * A387548  Expansion of (1/x) * Series_Reversion( x * (1 - x)^2 / (1 + x^2 /
 *          (1 - x)^2) ).
 * https://oeis.org/A387548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/2)} C(n+1,k) *
 *            C(3*n+1,n-2*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Handle the offset n=0 explicitly */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t n1 = n + 1;
    idx_t n3 = 3 * n + 1;
    idx_t maxk = n >> 1; /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t b1 = binom(n1, k);          /* C(n+1, k) */
        idx_t m = n - 2 * k;              /* n-2k */
        seq_t b2 = binom(n3, m);          /* C(3n+1, n-2k) */
        sum += b1 * b2;
    }
    return sum / (seq_t)(n + 1);
}
