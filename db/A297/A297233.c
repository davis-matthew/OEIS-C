/*
 * A297233  Total variation of base-11 digits of n; see Comments.
 * https://oeis.org/A297233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{i=1}^{len-1} |digit_i - digit_{i-1}|, where digit_0
 *            … digit_{len-1} are the base‑11 digits of n (digit_0 = least
 *            significant digit)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n == 0 gracefully; offset starts at 1, but this works
       for all non‑negative n. */
    idx_t t = n;
    int digits[64];   /* 64 > max number of base‑11 digits for 64‑bit n */
    int len = 0;

    /* Extract base‑11 digits (least significant first) */
    do {
        digits[len++] = t % 11;
        t /= 11;
    } while (t != 0);

    /* Compute total variation: sum of absolute differences of
       consecutive digits. */
    seq_t sum = 0;
    for (int i = 1; i < len; ++i) {
        int d1 = digits[i];
        int d0 = digits[i - 1];
        int diff = d1 > d0 ? d1 - d0 : d0 - d1;
        sum += diff;
    }

    return sum;
}
