/*
 * A284810  Fixed points of the transform A284809.
 * https://oeis.org/A284810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed points of transform A284809.
   a(n) is the n‑th integer whose base‑9 digits satisfy
   d_i + d_{k-1-i} = 8 for all i (the middle digit is 4 for odd
   lengths).  The function works for any n ≥ 1. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; guard against illegal call */
        return 0;

    /* Find the length of the base‑9 representation of a(n). */
    idx_t remaining = n - 1;  /* zero–based rank within the whole sequence */
    idx_t len = 1;            /* current length being examined */

    while (1) {
        idx_t count_len;
        if (len == 1) {
            count_len = 1;          /* only digit 4 */
        } else {
            idx_t m = len / 2;      /* floor(len/2) */
            /* compute 9^(m‑1) */
            idx_t pow9 = 1;
            for (idx_t i = 1; i < m; ++i)
                pow9 *= 9;
            count_len = 8 * pow9;   /* 8 options for the leading digit,
                                       9 options for each of the remaining (m‑1) pairs */
        }
        if (remaining < count_len)
            break;
        remaining -= count_len;
        ++len;
    }

    if (len == 1)              /* only the number 4 */
        return (seq_t)4;

    /* Remaining is the rank within numbers of length 'len'. */
    idx_t m = len / 2;          /* floor(len/2) – number of mirrored pairs */
    /* compute 9^(m‑1) again for digit extraction */
    idx_t pow9 = 1;
    for (idx_t i = 1; i < m; ++i)
        pow9 *= 9;

    idx_t lead_idx = remaining / pow9;        /* 0 .. 7 */
    idx_t lead = (uint8_t)(lead_idx + 1);      /* 1 .. 8 */
    idx_t rest = remaining % pow9;            /* remaining digits for the inner pairs */

    /* Store the first half of the digits (excluding the leading digit). */
    uint8_t first_half[64] = {0};            /* enough for all practical lengths */
    first_half[0] = (uint8_t)lead;
    for (idx_t pos = 1; pos < m; ++pos) {
        first_half[pos] = (uint8_t)(rest % 9);
        rest /= 9;
    }

    /* Assemble the full base‑9 digits and convert to decimal. */
    seq_t val = 0;
    for (idx_t i = 0; i < len; ++i) {
        uint8_t d;
        if (i < m) {                     /* first half */
            d = first_half[i];
        } else if ((len & 1) && (i == m)) {   /* middle digit for odd length */
            d = 4;
        } else {                         /* second half, mirrored and complemented */
            d = 8 - first_half[len - 1 - i];
        }
        val = val * 9 + d;
    }

    return val;
}
