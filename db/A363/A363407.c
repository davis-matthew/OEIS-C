/*
 * A363407  Sum of divisors of 4*n-3 of form 4*k+3.
 * https://oeis.org/A363407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d | (4*n-3), d ≡ 3 (mod 4)} d
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of divisors of 4*n-3 that are ≡ 3 (mod 4) */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = 4 * n - 3;      /* number to factorise */
    seq_t sum = 0;

    /* iterate up to √m */
    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            idx_t d1 = i;
            idx_t d2 = m / i;

            if ((d1 & 3) == 3)        /* d1 ≡ 3 (mod 4) */
                sum += (seq_t)d1;

            if (d2 != d1 && (d2 & 3) == 3) /* d2 distinct and 3 (mod 4) */
                sum += (seq_t)d2;
        }
    }

    return sum;
}
