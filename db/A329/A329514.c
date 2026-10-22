/*
 * A329514  Expansion of g.f.: (2*x^5-3*x^4+x^3-2*x^2-1)*(1+x)^2/(x-1).
 * https://oeis.org/A329514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = –∑_{k=0}^{min(n,7)} p_k where P(x) =
 *            (2*x^5-3*x^4+x^3-2*x^2-1)*(1+x)^2 expands to P(x) = –1 –2*x
 *            –3*x^2 –3*x^3 –3*x^4 –3*x^5 + x^6 + 2*x^7
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* coefficients of the polynomial P(x) above */
    static const int coeff[8] = { -1, -2, -3, -3, -3, -3, 1, 2 };

    /* For n ≥ 7 the partial sum becomes –12, so a(n) = 12. */
    if (n >= 7)
        return (seq_t)12;

    int sum = 0;
    for (idx_t i = 0; i <= n; ++i)
        sum += coeff[(size_t)i];

    /* sum is negative; negate it to obtain the correct unsigned value. */
    return (seq_t)(-sum);
}
