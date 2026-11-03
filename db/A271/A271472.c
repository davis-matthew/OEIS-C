/*
 * A271472  Binary representation of n in base i-1.
 * https://oeis.org/A271472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the binary expansion of the ordinary integer n in base
 *            (i-1), written as a decimal integer whose digits are the binary
 *            digits of the expansion (most significant bit first)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 19, u64 n <= 819,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 42 terms through n = 8887 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

seq_t a(idx_t n)
{
    /* Special case n==0 */
    if (n == 0) {
        return seq_from_str("0");
    }

    /* Use signed 128-bit integers for intermediate calculation */
    __int128 a_real = (__int128)n;
    __int128 b_real = 0;

    /* Store the digits (LSB first) */
    uint8_t bits[512];
    int bits_len = 0;

    while (a_real != 0 || b_real != 0) {
        /* r = parity of (a + b) */
        int r = (int)((a_real + b_real) & 1);
        bits[bits_len++] = r;

        __int128 a_minus_r = a_real - r;
        /* next values after division by (i-1) = -1 + i */
        __int128 new_a = (b_real - a_minus_r) / 2;
        __int128 new_b = (-a_minus_r - b_real) / 2;

        a_real = new_a;
        b_real = new_b;
    }

    /* Convert bits to a decimal string (most significant first) */
    char str[513];  /* bits_len <= 512, plus null terminator */
    for (int i = 0; i < bits_len; ++i) {
        str[bits_len - 1 - i] = '0' + bits[i];
    }
    str[bits_len] = '\0';

    return seq_from_str(str);
}
