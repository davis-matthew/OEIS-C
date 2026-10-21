/*
 * A262571  Concatenation of the numbers from 2 to n.
 * https://oeis.org/A262571
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of integers 2,3,…,n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    seq_t result = 0;

    for (idx_t k = 2; k <= n; ++k)
    {
        uint64_t d = digit_count(k);
        result = result * pow10(d) + (seq_t)k;
    }

    return result;
}
