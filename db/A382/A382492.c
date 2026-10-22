/*
 * A382492  a(n) is the least number that has exactly n 3-smooth divisors.
 * https://oeis.org/A382492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Min_{d|n} (2^(d-1) * 3^(n/d-1))
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    idx_t i;
    seq_t best = 0;       /* 0 means not set yet */

    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* candidate with divisor d1 */
            seq_t val1 = pow2(d1 - 1) * pow3(d2 - 1);
            if (best == 0 || val1 < best) best = val1;

            /* candidate with divisor d2 (avoid duplicate if square) */
            if (d1 != d2) {
                seq_t val2 = pow2(d2 - 1) * pow3(d1 - 1);
                if (best == 0 || val2 < best) best = val2;
            }
        }
    }
    return best;
}
