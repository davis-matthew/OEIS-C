/*
 * A384102  Least x in absolute value, such that there exists y, |x| >= |y| >
 *          0, such that n = |6xy + x + y|, or 0 if no such x exists. Choose x
 *          > 0 if x and -x are both possible.
 * https://oeis.org/A384102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = least integer x (|x| minimal) for which there exists
 *            non‑zero integer y with |x|>=|y| and |6xy + x + y| = n. Choose
 *            +x if both +x and -x are possible. Return 0 if no such x
 * Range:     i8 n <= 32, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* out of range, safety */
    seq_t n_val = (seq_t)n;

    /* Upper bound on |x| suggested by OEIS:  a(n) <= (6*n-1)/5 */
    idx_t bound = (6ULL * n - 1) / 5 + 1;

    for (idx_t k = 1; k <= bound; ++k)
    {
        /* Test positive x = +k */
        seq_t x_pos = (seq_t)k;
        seq_t denom_pos = 6 * x_pos + 1;           /* 6x+1 */

        for (int s = -1; s <= 1; s += 2)          /* V = ±n   */
        {
            seq_t numerator = (seq_t)s * n_val - x_pos;
            if (numerator % denom_pos == 0)
            {
                seq_t y = numerator / denom_pos;
                if (y != 0)
                {
                    seq_t abs_y = y < 0 ? -y : y;
                    if (abs_y <= x_pos)
                        return x_pos;          /* smallest positive x found */
                }
            }
        }

        /* Test negative x = -k */
        seq_t x_neg = -(seq_t)k;
        seq_t denom_neg = 6 * x_neg + 1;           /* 1 - 6k */

        for (int s = -1; s <= 1; s += 2)
        {
            seq_t numerator = (seq_t)s * n_val - x_neg;   /* s*n - (-k) = s*n + k */
            if (numerator % denom_neg == 0)
            {
                seq_t y = numerator / denom_neg;
                if (y != 0)
                {
                    seq_t abs_y = y < 0 ? -y : y;
                    if (abs_y <= (seq_t)k)
                        return x_neg;          /* smallest negative x if no positive found */
                }
            }
        }
    }
    return 0;                                   /* no solution */
}
