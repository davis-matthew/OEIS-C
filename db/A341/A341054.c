/*
 * A341054  For any number n with balanced ternary expansion (d_1, ..., d_k),
 *          the balanced ternary expansion of a(n), say (t_1, ..., t_k),
 *          satisfies t_m = d_1 + ... + d_m mod 3 for m = 1..k.
 * https://oeis.org/A341054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by converting n to balanced ternary, taking the
 *            cumulative sum of its digits modulo 3 (with 2 treated as -1),
 *            and interpreting the resulting digits as a balanced ternary
 *            number
 * Range:     u8 n <= 170, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n to balanced ternary digits (least‑significant first). */
    int8_t digits[128];
    int len = 0;

    if (n == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        idx_t temp = n;
        while (temp > 0) {
            idx_t r = temp % 3;
            temp /= 3;
            if (r == 0) {
                digits[len++] = 0;
            } else if (r == 1) {
                digits[len++] = 1;
            } else { /* r == 2 */
                digits[len++] = -1;
                temp += 1;          /* carry */
            }
        }
    }

    /* Compute a(n) from the digits in most‑significant order. */
    seq_t result = 0;
    int cum = 0;          /* cumulative sum of d_i */
    for (int i = len - 1; i >= 0; --i) {
        cum += digits[i];

        int mod = cum % 3;
        if (mod < 0) mod += 3;          /* make mod non‑negative */

        int t = (mod == 2) ? -1 : mod;  /* 0 -> 0, 1 -> 1, 2 -> -1 */

        result = result * 3 + (seq_t)t;
    }

    return result;
}
