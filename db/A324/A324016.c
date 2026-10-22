/*
 * A324016  N-digit substring of 81654327 taken from the left.
 * https://oeis.org/A324016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini, base
 * Formula:   a(n) = floor(81654327 / 10^(8-n)) (off­set 1)
 * Range:     u8 none, u16 none, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The constant 81654327 has 8 decimal digits. */
    static const seq_t base = 81654327;
    static const idx_t len = 8;   /* number of digits in base */

    /* For n outside the defined range return 0 (undefined). */
    if (n == 0 || n > len)
        return 0;

    /* Compute 10^(len-n) – exponent is at most 7, so a tiny loop is fine. */
    seq_t denom = 1;
    for (idx_t i = 0; i < (len - n); ++i)
        denom *= 10;

    return base / denom;
}
