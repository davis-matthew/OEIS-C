/*
 * A380350  In the ternary expansion of n, from left to right: replace the
 *          second, fourth, sixth, etc. nonzero digit, says d, by 3-d.
 * https://oeis.org/A380350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is obtained from the ternary expansion of n by replacing
 *            the 2nd, 4th, 6th, … non‑zero ternary digits (reading left to
 *            right) with 3 – d, where d is the original digit (1 or 2)
 * Range:     u8 n <= 251, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* seq_t and idx_t are supplied by the build; no definitions here. */
seq_t a(idx_t n)
{
    /* A tiny stack is enough: 64‑bit numbers have at most 39 ternary digits. */
    unsigned char stack[64];
    size_t len = 0;

    /* Extract ternary digits (least significant first). */
    while (n > 0) {
        stack[len++] = (unsigned char)(n % 3);
        n /= 3;
    }

    seq_t result = 0;
    size_t nonzero = 0;

    /* Process digits from most significant to least. */
    for (size_t i = len; i > 0; --i) {
        unsigned char d = stack[i - 1];
        if (d != 0) {
            ++nonzero;
            if ((nonzero & 1) == 0)          /* 2nd, 4th, ... non‑zero digit */
                d = (unsigned char)(3 - d);   /* flip 1 ↔ 2 */
        }
        result = result * 3 + d;
    }

    return result;
}
