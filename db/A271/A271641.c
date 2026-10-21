/*
 * A271641  Quantity of ancestors for n (in the sense explained below).
 * https://oeis.org/A271641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the number of decimal strings a_0 a_1 … a_k (k+1 digits)
 *            with a_0 non‑zero such that |a_i - a_{i+1}| equals the i‑th
 *            decimal digit of n for all i from 0 to k−1. This is computed by
 *            a simple DP over the digits of n
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Extract decimal digits of n into digits[0..L-1] (most significant first) */
    int rev[20]; /* enough for any 64‑bit value */
    int revlen = 0;
    if (n == 0) {
        rev[revlen++] = 0;
    } else {
        idx_t t = n;
        while (t > 0) {
            rev[revlen++] = (int)(t % 10);
            t /= 10;
        }
    }
    int L = revlen;
    int digits[20];
    for (int i = 0; i < L; ++i)
        digits[i] = rev[L - 1 - i];

    /* DP: dp_cur[a] = number of ways to reach current digit a */
    seq_t dp_cur[10] = {0};
    seq_t dp_next[10] = {0};

    /* first digit cannot be zero */
    for (int a = 1; a <= 9; ++a)
        dp_cur[a] = 1;

    for (int pos = 0; pos < L; ++pos) {
        int d = digits[pos];
        /* reset next array */
        for (int i = 0; i < 10; ++i)
            dp_next[i] = 0;

        for (int a = 0; a < 10; ++a) {
            seq_t ways = dp_cur[a];
            if (!ways)
                continue;
            if (d == 0) {
                /* only one transition: a -> a */
                dp_next[a] += ways;
            } else {
                int b1 = a + d;
                if (b1 <= 9)
                    dp_next[b1] += ways;
                int b2 = a - d;
                if (b2 >= 0)
                    dp_next[b2] += ways;
            }
        }
        /* copy dp_next to dp_cur for the next iteration */
        for (int i = 0; i < 10; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* sum all possibilities for the last digit */
    seq_t total = 0;
    for (int i = 0; i < 10; ++i)
        total += dp_cur[i];

    return total;
}
