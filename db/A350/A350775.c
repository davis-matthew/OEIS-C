/*
 * A350775  The balanced ternary expansion of a(n) is obtained by multiplying
 *          adjacent digits in the balanced ternary expansion of n: a(Sum_{k
 *          >= 0} t_k * 3^k) = Sum_{k >= 0} t_k * t_{k+1} * 3^k (with -1 <=
 *          t_k <= 1 for any k >= 0).
 * https://oeis.org/A350775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = Σ_{k≥0} t_k * t_{k+1} * 3^k where t_k are the balanced
 *            ternary digits (−1,0,1) of n
 * Range:     i8 n <= 370, i16 n <= 9841, i32 n <= 9841, i64 n <= 9841,
 *            i128 n <= 9841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>           /* for int8_t */

seq_t a(idx_t n)
{
    /* convert n to balanced ternary digits */
    int8_t digits[64];       /* enough for 64‑bit n */
    int len = 0;
    idx_t nn = n;
    while (nn != 0) {
        idx_t r = nn % 3;
        nn /= 3;
        int8_t d;
        if (r == 2) {        /* convert 2 → −1 and carry 1 */
            d = -1;
            nn += 1;
        } else {
            d = (int8_t)r;   /* 0 or 1 */
        }
        digits[len++] = d;
    }
    digits[len] = 0;          /* t_{len} = 0 for convenience */

    /* compute the sum Σ t_k * t_{k+1} * 3^k */
    seq_t ans = 0;
    seq_t pow3 = 1;
    for (int k = 0; k < len; ++k) {
        ans += (seq_t)digits[k] * digits[k + 1] * pow3;
        pow3 *= 3;
    }
    return ans;
}
