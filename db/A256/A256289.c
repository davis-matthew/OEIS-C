/*
 * A256289  Apply the transformation 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8
 *          -> 0 to the digits of n written in base 9; do not convert back to
 *          base 10.
 * https://oeis.org/A256289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal number obtained by incrementing each base‑9 digit
 *            of n by 1 modulo 9
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   _shift_base9_to_dec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_shift_base9_to_dec.h"

/* OEIS function */
seq_t a(idx_t n)
{
    return _shift_base9_to_dec(n);
}
