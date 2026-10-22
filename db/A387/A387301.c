/*
 * A387301  a(n) = Sum_{k=0..floor(n/2)} binomial(n+k-1,k) *
 *          binomial(n+k-1,n-2*k).
 * https://oeis.org/A387301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n+k-1,k) *
 *            binomial(n+k-1,n-2*k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;

    seq_t res = 0;
    idx_t maxk = n / 2;            /* floor(n/2) */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t big = n + k - 1;     /* n + k - 1 */
        seq_t term1 = binomial(big, k);          /* C(n+k-1, k) */
        seq_t term2 = binomial(big, n - 2 * k);  /* C(n+k-1, n-2k) */
        res += term1 * term2;
    }
    return res;
}
