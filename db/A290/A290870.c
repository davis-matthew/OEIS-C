/*
 * A290870  a(n) is the number of ways to represent n as n = x*y + y*z + z*x
 *          where 0 < x < y < z.
 * https://oeis.org/A290870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to n = x*y + y*z + z*x with 0 < x < y
 *            < z
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below counts the number of ordered triples (x,y,z)
   of positive integers satisfying n = xy + yz + zx and x < y < z.
   It uses brute force over the possible ranges for x and y, which
   is fast enough for n up to about 10 000 (a few hundred thousand
   iterations). No external libraries are required. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t count = 0;

    /* x must satisfy 3*x^2 < n for any solution */
    for (idx_t x = 1; 3 * x * x <= n; ++x) {
        /* For a given x, y must be > x and satisfy n - x*y > 0 */
        idx_t y_max = n / x - 1;   /* ensures n - x*y > 0 */
        if (y_max <= x) continue;  /* no valid y in this row */

        for (idx_t y = x + 1; y <= y_max; ++y) {
            seq_t num = (seq_t)n - (seq_t)x * y;
            seq_t den = (seq_t)x + y;
            if (num % den == 0) {
                seq_t z = num / den;
                if (z > y) ++count;
            }
        }
    }
    return count;
}
