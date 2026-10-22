/*
 * A393250  a(n) = Sum_{k=0..floor(n/6)} binomial(2*k,k) *
 *          binomial(n-3*k,3*k).
 * https://oeis.org/A393250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/6⌋} binomial(2k,k) * binomial(n-3k,3k)
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 40, u64 n <= 76, u128 n <= 149,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t limit = n / 6;
    seq_t result = 0;

    for (idx_t k = 0; k <= limit; ++k)
    {
        seq_t c1 = binomial(2 * k, k);
        seq_t c2 = binomial(n - 3 * k, 3 * k);
        result += c1 * c2;
    }

    return result;
}
