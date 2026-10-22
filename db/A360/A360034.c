/*
 * A360034  Binary representation of -n in base i-1.
 * https://oeis.org/A360034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 12, u64 n <= 204, u128 n <= 9583,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  For n ≥ 0, a(n) is the binary representation of –n written in base –4.
    Each base––4 digit is replaced by a 4–bit block:
       0 → 0000
       1 → 0001
       2 → 1100
       3 → 1101
    The concatenated bit string is interpreted as a decimal integer.
    (This matches the OEIS definition of A360034.) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: a(0) = 0 */
    if (n == 0) {
        return 0;
    }

    /* Convert –n to base –4.  Use signed 64‑bit arithmetic – n is ≤ 10000 in tests. */
    int64_t val = -(int64_t)n;          /* signed value of –n       */
    uint8_t digits[64];                 /* digits in little‑endian order (LS first) */
    int len = 0;

    while (val != 0) {
        int r = (int)(val % 4);         /* remainder in –3 … 3       */
        if (r < 0) {
            r += 4;                     /* make it 0 … 3             */
        }
        val = (val - r) / -4;           /* next quotient, base –4   */
        digits[len++] = (uint8_t)r;     /* store digit               */
    }

    /* Mapping from base––4 digit to its 4‑bit pattern. */
    static const uint8_t map4[4] = { 0x0, 0x1, 0xC, 0xD };

    /* Build the decimal integer that corresponds to the concatenated bit string. */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {          /* most significant digit first */
        uint8_t pat = map4[digits[i]];
        for (int bit = 3; bit >= 0; --bit) {       /* bit 3 is the most significant bit of the block */
            int b = (pat >> bit) & 1;
            res = res * 10 + b;
        }
    }

    return res;
}
