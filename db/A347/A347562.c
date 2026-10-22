/*
 * A347562  Number of ways to write n as 16^w + x^2 + (y^2 + 23*z^4)/324,
 *          where w,x,y,z are nonnegative integers.
 * https://oeis.org/A347562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of quadruples (w,x,y,z) of nonnegative integers
 *            such that n = 16^w + x^2 + (y^2 + 23*z^4)/324
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root (floor) of a seq_t number. */
static seq_t isqrt_seq(seq_t n)
{
    seq_t low = 0;
    seq_t high = n;
    while (low <= high) {
        seq_t mid = low + ((high - low) >> 1);
        seq_t mid_sq = mid * mid;
        if (mid_sq == n) return mid;
        if (mid_sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;      /* floor sqrt */
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t n_seq = (seq_t)n;

    /* Iterate over w: pw = 16^w. */
    for (seq_t pw = 1; pw <= n_seq; pw *= 16) {
        seq_t rem1 = n_seq - pw;
        seq_t xmax = isqrt_seq(rem1);
        for (seq_t x = 0; x <= xmax; ++x) {
            seq_t x_sq = x * x;
            seq_t rem2 = rem1 - x_sq;          /* 16^w + x^2 accounted for */
            seq_t target = 324 * rem2;         /* y^2 + 23*z^4 must equal this */

            /* Scan over z. */
            for (seq_t z = 0;; ++z) {
                seq_t z2   = z * z;
                seq_t z4   = z2 * z2;
                seq_t z_term = 23 * z4;
                if (z_term > target) break;    /* larger z would only increase */

                seq_t y_sq = target - z_term;
                seq_t y = isqrt_seq(y_sq);
                if (y * y == y_sq) {          /* y is integer */
                    ++count;                   /* found a valid quadruple */
                }
            }
        }
    }
    return count;
}
