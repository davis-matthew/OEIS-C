/*
 * A245570  Rectangular array A read by (upward) antidiagonals: A(n,k) =
 *          n/gcd(n,10^k), n,k >= 1.
 * https://oeis.org/A245570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(n,k) = n / gcd(n, 10^k) (array read by upward
 *            antidiagonals)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Find m such that m*(m+1)/2 >= n */
    idx_t low = 0, high = 1;
    while ((__uint128_t)high * (high + 1) / 2 < n)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;                     /* m >= 1 */

    __uint128_t prev_T = (__uint128_t)(m - 1) * m / 2;  /* T_{m-1} */
    idx_t d = (idx_t)(n - prev_T - 1);                 /* 0‑based offset in this diagonal */
    idx_t i = m - d;                                  /* row index (n) */
    idx_t j = d + 1;                                  /* column index (k) */

    /* Count powers of 2 in i */
    unsigned int v2 = 0;
    idx_t tmp = i;
    while ((tmp & 1ULL) == 0) {
        v2++;
        tmp >>= 1;
    }

    /* Count powers of 5 in i */
    unsigned int v5 = 0;
    tmp = i;
    while (tmp % 5 == 0 && tmp > 0) {
        v5++;
        tmp /= 5;
    }

    idx_t p = (v2 < j) ? v2 : j;   /* min(v2(i), j) */
    idx_t q = (v5 < j) ? v5 : j;   /* min(v5(i), j) */

    seq_t gcd_factor = pow_uint(2, p);
    if (q > 0)
        gcd_factor = gcd_factor * pow_uint(5, q);

    return ((seq_t)i) / gcd_factor;
}
