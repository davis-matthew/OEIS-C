/*
 * A363900  Expansion of Sum_{k>0} k * x^(4*k) / (1 - x^(5*k)).
 * https://oeis.org/A363900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d | n, n/d == 4 mod 5} d
 * Range:     u8 n <= 647, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate over divisor pairs using the bound n / d */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;           /* smaller divisor */
            idx_t d2 = n / i;       /* corresponding larger divisor */

            /* check d1 */
            idx_t t1 = d2;           /* n/d1 */
            if (t1 % 5 == 4)
                sum += (seq_t)d1;

            /* check d2 (avoid double‑counting for squares) */
            if (d1 != d2) {
                idx_t t2 = d1;       /* n/d2 */
                if (t2 % 5 == 4)
                    sum += (seq_t)d2;
            }
        }
    }
    return sum;
}
