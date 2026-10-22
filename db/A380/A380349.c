/*
 * A380349  In the ternary expansion of n, from left to right: replace the
 *          first, third, fifth, etc. nonzero digit, says d, by 3-d.
 * https://oeis.org/A380349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is obtained from the ternary expansion of n by flipping the
 *            1st, 3rd, 5th, … non‑zero digits (1 ↔ 2)
 * Range:     u8 n <= 242, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case n = 0. */
    if (n == 0) return 0;

    /* digits[0] holds least significant base‑3 digit. */
    int digits[64];
    int len = 0;
    idx_t u = n;
    while (u > 0) {
        digits[len++] = (int)(u % 3);
        u /= 3;
    }

    /* Flip first, third, fifth, … non‑zero digits. */
    int nonzero = 0;
    for (int i = len - 1; i >= 0; --i) {
        if (digits[i] != 0) {
            if ((nonzero & 1) == 0) {          /* 0,2,4,… -> flip */
                digits[i] = 3 - digits[i];
            }
            ++nonzero;
        }
    }

    /* Reconstruct the number from most significant to least. */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        res = res * 3 + (seq_t)digits[i];
    }
    return res;
}
