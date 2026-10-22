/*
 * A361612  Decimal expansion of sqrt(10) truncated to n places (after the
 *          decimal point).
 * https://oeis.org/A361612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(10) * 10^n)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a digit‑by‑digit square root algorithm, applying
   128‑bit intermediates when seq_t is no larger than 64 bits, and
   seq_t operations otherwise. This guarantees an exact result for
   all n for which the value fits in the target integer type. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 64‑bit (or smaller) implementation using 128‑bit arithmetic. */
    if (sizeof(seq_t) <= 8) {
        __uint128_t rem = 10;          /* initial remainder = 10 */
        __uint128_t res = 0;           /* running result */

        for (idx_t i = 0; i <= n; ++i) {
            /* find largest digit d with (20*res + d)*d <= rem */
            uint8_t d = 9;
            for (;;) {
                __uint128_t t = (res * 20 + d) * d;
                if (t <= rem) break;
                --d;
            }

            rem -= (res * 20 + d) * d;
            res = res * 10 + d;

            /* bring down the next pair of zeros, except after the last digit */
            if (i < n) rem *= 100;
        }
        return (seq_t)res;
    }

    /* arbitrary‑precision implementation using seq_t operations. */
    seq_t rem = 10;              /* initial remainder = 10 */
    seq_t res = 0;               /* running result */

    for (idx_t i = 0; i <= n; ++i) {
        /* find largest digit d with (20*res + d)*d <= rem */
        uint8_t d = 9;
        for (;;) {
            seq_t t = (res * 20 + d) * d;
            if (t <= rem) break;
            --d;
        }

        rem = rem - (res * 20 + d) * d;
        res = res * 10 + d;

        /* bring down the next pair of zeros, except after the last digit */
        if (i < n) rem = rem * 100;
    }
    return res;
}
