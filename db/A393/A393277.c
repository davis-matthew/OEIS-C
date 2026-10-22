/*
 * A393277  a(n) = Sum_{k=0..floor(n/4)} binomial(2*k,k) *
 *          binomial(n-2*k-1,n-4*k).
 * https://oeis.org/A393277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binomial(2k, k) * binomial(n-2k-1, n-4k)
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 69, u128 n <= 133,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return 1;

    seq_t sum = 0;
    idx_t maxk = n / 4;        /* floor(n/4) */

    for (idx_t k = 0; k <= maxk; ++k)
        sum += binomial(2 * k, k) * binomial(n - 2 * k - 1, n - 4 * k);

    return sum;
}
