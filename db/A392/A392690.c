/*
 * A392690  a(n) = denominator( (-1)^(n-1)*H(2*n)/(2*n + 1) ), where H(n) is
 *          the n-th harmonic number.
 * https://oeis.org/A392690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator( (-1)^(n-1)*H(2*n)/(2*n + 1) )
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* m = 2*n */
    idx_t m = 2 * n;

    /* Compute L = lcm(1,2,…,m) */
    seq_t L = 1;
    for (idx_t k = 2; k <= m; ++k) {
        seq_t k_t = k;
        seq_t g = gcd(L, k_t);
        L = (L / g) * k_t;
    }

    /* Compute numerator p = sum_{k=1}^{m} L/k */
    seq_t num = 0;
    for (idx_t k = 1; k <= m; ++k) {
        num += L / (seq_t)k;
    }

    /* Denominator before reduction: L * (2*n + 1) */
    seq_t denom = L * (seq_t)(2 * n + 1);

    /* Reduce fraction */
    seq_t g2 = gcd(num, denom);
    denom /= g2;

    return denom;
}
