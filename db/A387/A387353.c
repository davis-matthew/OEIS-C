/*
 * A387353  a(n) = Sum_{k=0..floor(n/2)} binomial(n+k-1,k) *
 *          binomial(n+2*k-1,n-2*k).
 * https://oeis.org/A387353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n+k-1,k) *
 *            binomial(n+2*k-1,n-2*k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n >> 1;          /* floor(n/2) */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term1 = binom(n + k - 1, k);
        seq_t term2 = binom(n + 2 * k - 1, n - 2 * k);
        sum += term1 * term2;
    }
    return sum;
}
