/*
 * A348192  a(0) = 0; for n >= 1, a(n) = 1 + a(n - GCD(n, digital sum(n))).
 * https://oeis.org/A348192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; For n >= 1: a(n) = 1 + a(n - GCD(n, digital_sum(n)))
 *            (Number of steps to reach zero by repeatedly subtracting gcd of
 *            the current value and its sum of decimal digits.)
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   digit_sum, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    seq_t steps = 0;          /* result stored in seq_t */
    idx_t cur = n;            /* working value, fits in idx_t */

    while (cur > 0) {
        seq_t sn    = (seq_t)cur;
        seq_t d     = gcd(sn, digit_sum(sn));   /* 1 <= d <= sn */
        cur -= (idx_t)d;        /* guaranteed to be >= 0 */
        steps += 1;
    }
    return steps;
}
