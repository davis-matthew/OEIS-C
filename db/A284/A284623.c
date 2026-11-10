/*
 * A284623  Positions of 0 in A284622; complement of A284626.
 * https://oeis.org/A284623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A284622, the {0011->0}
 *            transform of Thue-Morse (direct scan, greedy left to right)
 * Range:     u8 n <= 141, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9878 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t x = 0;       /* index in Thue-Morse word */
    idx_t y = 1;       /* 1-based output position */
    idx_t zeros = 0;

    for (;;) {
        int b0 = __builtin_parityll((unsigned long long)x);
        int b1 = __builtin_parityll((unsigned long long)(x + 1));
        int b2 = __builtin_parityll((unsigned long long)(x + 2));
        int b3 = __builtin_parityll((unsigned long long)(x + 3));

        if (b0 == 0 && b1 == 0 && b2 == 1 && b3 == 1) {
            /* 0011 -> single 0 */
            ++zeros;
            if (zeros == n) return (seq_t)y;
            x += 4;
            ++y;
        } else {
            if (b0 == 0) {
                ++zeros;
                if (zeros == n) return (seq_t)y;
            }
            x += 1;
            ++y;
        }
    }
}
