/*
 * A364032  Expansion of Sum_{k>0} x^(3*k) / (1 + x^(4*k)).
 * https://oeis.org/A364032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d | n, d == 3 (mod 4)} (-1)^((d-3)/4)
 * Range:     i8 n <= 101, i16 n <= 101, i32 n <= 101, i64 n <= 101,
 *            i128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* for safety, though offset is 1 */
    seq_t sum = 0;

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* first divisor d */
            if ((d & 3) == 3) {   /* d % 4 == 3 */
                idx_t k = (d - 3) / 4;
                seq_t term = (k & 1) ? -1 : 1;
                sum += term;
            }

            idx_t d2 = n / d;    /* corresponding divisor n/d */
            if (d2 != d) {
                if ((d2 & 3) == 3) {
                    idx_t k = (d2 - 3) / 4;
                    seq_t term = (k & 1) ? -1 : 1;
                    sum += term;
                }
            }
        }
    }
    return sum;
}
