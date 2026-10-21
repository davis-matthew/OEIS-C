/*
 * A297244  Up-variation of the base-15 digits of n; see Comments.
 * https://oeis.org/A297244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns a(n) for OEIS A297244: 
   the up‑variation of the base‑15 digits of n.
   The algorithm converts n to base‑15 (least significant digit first)
   and sums all positive differences when moving towards more significant digits. */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* base‑15 digits of n, least significant first */
    uint8_t digits[64];
    size_t len = 0;

    idx_t tmp = n;
    while (tmp > 0 && len < sizeof(digits))
    {
        digits[len++] = (uint8_t)(tmp % 15U);
        tmp /= 15U;
    }

    seq_t uv = 0;
    for (size_t i = 0; i + 1 < len; ++i)
    {
        uint8_t high = digits[i];
        uint8_t low  = digits[i + 1];
        if (low < high)          /* digit decreases towards more significant place */
            uv += (seq_t)(high - low);
    }
    return uv;
}
