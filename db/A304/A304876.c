/*
 * A304876  L.g.f.: log(Product_{k>=1} (1 + x^(k*(k+1)/2))) = Sum_{n>=1}
 *          a(n)*x^n/n.
 * https://oeis.org/A304876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n, d triangular} d * (-1)^(n/d - 1)
 * Range:     i8 n <= 104, i16 n <= 8977, i32 n <= 8977, i64 n <= 8977,
 *            i128 n <= 8977, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; return 0 for safety */

    seq_t sum = 0;
    for (idx_t k = 1; ; ++k) {
        /* t = k*(k+1)/2, computed with 128‑bit arithmetic to avoid overflow */
        __uint128_t t128 = (__uint128_t)k * (k + 1) / 2;
        if (t128 > n) break;               /* no more triangular divisors */
        idx_t t = (idx_t)t128;

        if (n % t == 0) {
            idx_t m = n / t;                /* quotient n/t */
            if (m & 1)                     /* m odd → sign + */
                sum += (seq_t)t;
            else                           /* m even → sign - */
                sum -= (seq_t)t;
        }
    }
    return sum;
}
