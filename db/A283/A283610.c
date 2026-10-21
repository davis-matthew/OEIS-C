/*
 * A283610  Numbers n whose largest decimal digit is 7.
 * https://oeis.org/A283610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th positive integer whose largest decimal digit
 *            equals 7
 * Range:     u8 n <= 35, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   max_digit_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/max_digit_seq.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t val = 0;

    for (;; ++val)
    {
        if (max_digit_seq(val) == 7)
        {
            ++count;
            if (count == n) return val;
        }
    }
}
