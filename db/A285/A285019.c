/*
 * A285019  Numerator of (-1/3)^n*sqrt(Pi)/(Gamma(1/2 - n)*Gamma(1 + n)).
 * https://oeis.org/A285019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = C(2n,n) / (2^{v2(n)} * 3^{min(v3(n), n)}), where v2(n) =
 *            number of 1‑bits in n (binary sum of digits), v3(n) = (2·s_3(n)
 *            – s_3(2n))/2, and s_3(x) is the sum of base‑3 digits of x
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   sum_digits_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sum_digits_base3.h"

/* Count 1‑bits in an idx_t value */
static idx_t popcnt(idx_t x)
{
    idx_t cnt = 0;
    while (x)
    {
        x &= x - 1;
        cnt++;
    }
    return cnt;
}

/* Compute central binomial coefficient C(2n,n) */
static seq_t central_binomial(idx_t n)
{
    seq_t c = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        c = (c * (seq_t)(n + k)) / (seq_t)k;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Central binomial coefficient */
    seq_t cb = central_binomial(n);

    /* Exponent of 2 in the coefficient: v2 = popcnt(n) */
    idx_t v2 = popcnt(n);

    /* Exponents of 3 */
    idx_t s3_n   = sum_digits_base3(n);
    idx_t s3_2n  = sum_digits_base3(n * (idx_t)2);
    idx_t v3     = (2 * s3_n - s3_2n) / 2;          /* integer division */
    idx_t rem3   = v3 < n ? v3 : n;                /* min(v3, n) */

    /* Compute divisor g = 2^{v2} * 3^{rem3} */
    seq_t g = 1;
    for (idx_t i = 0; i < v2; ++i)
        g *= 2;
    for (idx_t i = 0; i < rem3; ++i)
        g *= 3;

    /* Result is the numerator after cancelling common factors */
    return cb / g;
}
