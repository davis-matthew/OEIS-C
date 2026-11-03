/*
 * A360287  a(n) is the concatenation of the positions of 1-bits in the binary
 *          expansion of the Gray code for n, when 1 is the rightmost
 *          position; a(0) = 0.
 * https://oeis.org/A360287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = concatenation of positions of 1‑bits in the binary Gray
 *            code of n (positions counted from 1 at the rightmost bit). a(0)
 *            = 0
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 681, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 68 terms through n = 9714 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/num_digits_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t g = n ^ (n >> 1);  /* Gray code of n */
    seq_t result = (seq_t)0;

    for (idx_t pos = 1; g; ++pos, g >>= 1) {
        if (g & 1) {
            /* multiply current result by 10^digits(pos) and add pos */
            seq_t mult = pow10((idx_t)num_digits_idx(pos));
            result = result * mult + (seq_t)pos;
        }
    }

    return result;
}
