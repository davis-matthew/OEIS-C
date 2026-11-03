/*
 * A242374  Number of digits in the decimal expansion of n^8.
 * https://oeis.org/A242374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of decimal digits of n^8
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 67 (2026-11-01)
 * Helpers:   ipow_idx, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses pow_int to raise n to the 8th power and digit_len to count digits */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/num_digits.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    seq_t p    = ipow_idx(base, 8);
    return num_digits(p);
}
