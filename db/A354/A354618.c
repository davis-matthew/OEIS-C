/*
 * A354618  a(n) = (sum of the digits of 5^n) - (sum of the digits of 2^n).
 * https://oeis.org/A354618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (sum of digits of 5^n) - (sum of digits of 2^n)
 * Range:     u8 n <= 112, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_sum_power.h"

seq_t a(idx_t n)
{
    seq_t s5 = digit_sum_power(5, n);
    seq_t s2 = digit_sum_power(2, n);
    return s5 - s2;
}
