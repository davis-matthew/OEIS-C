/*
 * A302487  Total domination number of the n-Apollonian network.
 * https://oeis.org/A302487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 for n = 1,2; a(n) = (3^(n-3) + 5) / 2 for n >= 3
 *            (recurrence a(n) = 4*a(n-1) - 3*a(n-2) for n > 4 is equivalent)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Base cases for the offset */
    if (n == 1u || n == 2u)
        return (seq_t)2;

    /* For n >= 3 compute (3^(n-3) + 5)/2 */
    idx_t exp = n - (idx_t)3;
    seq_t pow_val = seq_pow((seq_t)3, exp);

    /* (pow_val + 5) is guaranteed even, so division truncates correctly */
    return (pow_val + (seq_t)5) / (seq_t)2;
}
