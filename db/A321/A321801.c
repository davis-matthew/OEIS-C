/*
 * A321801  Delete all consecutive identical decimal digits of n; write 0 if
 *          all digits disappear.
 * https://oeis.org/A321801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = delete all consecutive identical decimal digits of n;
 *            leading zeros are erased unless the result is 0
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Extract decimal digits of n into an array (most‑significant first). */
    int digits[20];
    int len = 0;
    idx_t temp = n;

    if (temp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (temp > 0) {
            digits[len++] = temp % 10;
            temp /= 10;
        }
        /* Reverse to most‑significant‑first order. */
        for (int i = 0; i < len / 2; ++i) {
            int t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    seq_t result = 0;
    bool started = false;   /* true once a non‑leading digit is kept */

    for (int i = 0; i < len; ) {
        int j = i;
        while (j + 1 < len && digits[j + 1] == digits[i]) {
            ++j;
        }
        int run_len = j - i + 1;

        if (run_len == 1) {
            int d = digits[i];
            if (!started && d == 0) {
                /* skip leading zero */
            } else {
                result = result * 10 + d;
                started = true;
            }
        }
        i = j + 1;
    }

    if (!started) {
        /* all digits removed or only leading zeros left */
        return 0;
    }
    return result;
}
