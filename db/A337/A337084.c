/*
 * A337084  a(n) is the smallest nonzero digit d whose product d*n will
 *          contain the digit d, or 0 if no such digit exists.
 * https://oeis.org/A337084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest nonzero digit d such that d*n contains the
 *            digit d; return 0 if no such digit exists
 * Range:     u8 n <= 37, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    for (uint8_t d = 1; d <= 9; ++d)
    {
        seq_t prod = ((seq_t)d) * ((seq_t)n);
        seq_t tmp = prod;
        while (tmp > 0)
        {
            if ((tmp % 10) == d)
                return d;
            tmp /= 10;
        }
    }
    return (seq_t)0;
}
