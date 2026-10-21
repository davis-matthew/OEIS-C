/*
 * A249182  Proceed counterclockwise on the outer keys of a numeric keypad
 *          (i.e., 1,2,3,6,9,8,7,4): first single digits, then concatenate two
 *          digits, then three, etc.
 * https://oeis.org/A249182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number obtained by concatenating k digits from the *
 *            cyclic order [1,2,3,6,9,8,7,4], starting at index (n-1) mod 8, *
 *            where k = floor((n-1)/8)+1
 * Range:     u8 n <= 18, u16 n <= 35, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint8_t keypad[8] = {1, 2, 3, 6, 9, 8, 7, 4};

seq_t a(idx_t n)
{
    idx_t idx   = n - 1;                    /* zero‑based position */
    idx_t block = idx / 8;                  /* which block (k-1) */
    idx_t start = idx % 8;                  /* starting index in keypad */

    seq_t value = 0;
    for (idx_t i = 0; i < block + 1; ++i)
    {
        uint8_t d = keypad[(start + i) % 8];
        value = value * (seq_t)10 + (seq_t)d;
    }
    return value;
}
