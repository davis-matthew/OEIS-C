/*
 * A383479  Number of lattice paths from (0,0) to (n,n) using steps
 *          (1,0),(3,0),(0,1).
 * https://oeis.org/A383479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n+k,k) *
 *            binomial(2*n-2*k,n-3*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t kmax = n / 3;
    for (idx_t k = 0; k <= kmax; ++k)
    {
        seq_t term1 = binom(n + k, k);                /* C(n+k, k) */
        seq_t term2 = binom(2 * n - 2 * k, n - 3 * k);/* C(2n-2k, n-3k) */
        seq_t term  = term1 * term2;
        result += term;
    }
    return result;
}
