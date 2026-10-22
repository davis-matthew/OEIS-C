/*
 * A393246  a(n) = Sum_{k=0..floor(n/4)} binomial(2*k,k) * binomial(n-k,3*k).
 * https://oeis.org/A393246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..floor(n/4)} binomial(2*k, k) * binomial(n-k,
 *            3*k)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 59, u128 n <= 115,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 4;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* binom is the helper that returns binomial(n, k) exactly. */
        seq_t c1 = binom(2 * k, k);
        seq_t c2 = binom(n - k, 3 * k);

        sum += c1 * c2;
    }

    return sum;
}
