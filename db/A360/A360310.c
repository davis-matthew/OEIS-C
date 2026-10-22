/*
 * A360310  a(n) = Sum_{k=0..floor(n/4)} binomial(n-1-3*k,n-4*k) *
 *          binomial(2*k,k).
 * https://oeis.org/A360310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..floor(n/4)} binomial(n-1-3*k, n-4*k) *
 *            binomial(2*k, k)
 * Range:     u8 n <= 15, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the supplied helper function binomial(idx_t n, idx_t k). */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) is defined to be 1. */
    if (n == 0)
        return 1;

    seq_t sum = 0;
    /* The range of k is from 0 to floor(n/4). */
    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t a1 = n - 1 - 3 * k;   /* n-1-3*k  */
        idx_t b1 = n - 4 * k;       /* n-4*k    */
        seq_t term1 = binomial(a1, b1);
        seq_t term2 = binomial(2 * k, k);
        sum = sum + term1 * term2;
    }
    return sum;
}
