/*
 * A372611  Expansion of (1 + 3*x) / ((1 - 2*x)*sqrt(1 - 4*x)).
 * https://oeis.org/A372611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A372611:  a(n) = 2*a(n-1) + binomial(2n,n) + 3*binomial(2n-2,n-1),
    a(0) = 1
    (uses the recurrence from the OEIS entry)
*/
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t prev = (seq_t)1;          /* a(0) */
    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        seq_t term = prev * (seq_t)2;
        term += binomial(i * (idx_t)2, i);
        term += (seq_t)3 * binomial((i * (idx_t)2) - (idx_t)2, i - (idx_t)1);
        prev = term;
    }
    return prev;
}
