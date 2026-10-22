/*
 * A338828  For any number with ternary representation (t(1), t(2), ...,
 *          t(k)), the ternary representation of a(n) is (abs(t(1)-t(k)),
 *          abs(t(2)-t(k-1)), ..., abs(t(k)-t(1))).
 * https://oeis.org/A338828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = ternary representation of n with digits *
 *            (abs(t(i)-t(k+1-i))) where t(1) ... t(k) are the base‑3 digits
 *            of n. * The result is the integer with those digits interpreted
 *            in base 3
 * Range:     u8 n <= 245, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Extract ternary digits of n into array `d`. */
    uint8_t d[64];            /* enough for n up to 2^64 < 3^40 */
    idx_t len = 0;

    if (n == 0) {
        d[0] = 0;
        len = 1;
    } else {
        idx_t t = n;
        while (t > 0) {
            d[len++] = (uint8_t)(t % 3);
            t /= 3;
        }
    }

    /* Compute the result digit by digit. */
    seq_t res = 0;
    seq_t pow = 1;                /* 3^i as seq_t */
    for (idx_t i = 0; i < len; ++i) {
        uint8_t left  = d[i];
        uint8_t right = d[len - 1 - i];
        uint8_t diff = left > right ? left - right : right - left;
        res += (seq_t)diff * pow;
        pow *= (seq_t)3;
    }
    return res;
}
