/*
 * A280823  Numbers with an odd number of digits and with an odd number of
 *          distinct digits.
 * https://oeis.org/A280823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth nonnegative integer having an odd number of
 *            decimal digits and an odd number of distinct decimal digits. The
 *            sequence starts with a(1)=0, a(2)=1,
 * Range:     u8 n <= 120, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   odd_len_and_distinct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/odd_len_and_distinct.h"

seq_t a(idx_t n)
{
    seq_t x = 0;
    idx_t count = 0;

    while (1) {
        if (odd_len_and_distinct(x)) {
            ++count;
            if (count == n) {
                return x;
            }
        }
        ++x;
    }
}
