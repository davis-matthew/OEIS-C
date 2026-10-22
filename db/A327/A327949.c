/*
 * A327949  Decimal expansion of deuteron g factor.
 * https://oeis.org/A327949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, more
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal digits of the deuteron g‑factor.
   The expansion is 0.8574382338(22) – first ten digits are
   8574382338, after which the digit 2 repeats forever. */
#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t digits[10] = {
        8, 5, 7, 4, 3, 8, 2, 3, 3, 8
    };

    if (n < 10)
        return digits[n];
    else
        return (seq_t)2;
}
