/*
 * A384103  a(n) = y with minimum |x| >= |y| > 0, such that n = |6xy + x + y|,
 *          or 0 if no such x, y exist. If x and -x are solutions, choose x >
 *          0 > y = -x.
 * https://oeis.org/A384103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = y where y is non‑zero, |x| ≥ |y| > 0 and * n = |6·x·y + x
 *            + y|. The minimal |x| is chosen, and for * a tie between x and
 *            –x the positive x is returned
 * Range:     i8 n <= 32, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* offset starts at 1 */

    const seq_t N = (seq_t)n;    /* cast to seq_t for arithmetic */

    for (idx_t k = 1; k <= n; ++k) {
        /* ----- positive x = k ----- */
        seq_t x     = (seq_t)k;
        seq_t denom = 6 * x + (seq_t)1;       /* 6k + 1 */

        /* test s = +N */
        seq_t num = N - x;                    /* N - x */
        if (num % denom == 0) {
            seq_t y = num / denom;
            if (y != 0 && (y >= 0 ? y : -y) <= x)
                return y;
        }
        /* test s = -N */
        num = -N - x;                         /* -N - x */
        if (num % denom == 0) {
            seq_t y = num / denom;
            if (y != 0 && (y >= 0 ? y : -y) <= x)
                return y;
        }

        /* ----- negative x = -k ----- */
        x     = -(seq_t)k;
        denom = 6 * x + (seq_t)1;             /* -6k + 1 */

        /* test s = +N  (N - x = N + k) */
        num = N - x;
        if (num % denom == 0) {
            seq_t y = num / denom;
            if (y != 0 && (y >= 0 ? y : -y) <= (seq_t)k)
                return y;
        }
        /* test s = -N  (-N - x = -N + k) */
        num = -N - x;
        if (num % denom == 0) {
            seq_t y = num / denom;
            if (y != 0 && (y >= 0 ? y : -y) <= (seq_t)k)
                return y;
        }
    }

    /* no admissible pair found */
    return 0;
}
