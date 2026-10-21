/*
 * A248699  Expansion of (2 + x + 5*x^2 + 3*x^3 + 3*x^4 + x^5) / ((1 - x^3) *
 *          (1 - x^4)) in powers of x.
 * https://oeis.org/A248699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * Let B[0..5] = {2, 1, 5, 3, 3, 1} (coefficients of the
 *            polynomial * 2 + x + 5*x^2 + 3*x^3 + 3*x^4 + x^5). * For n ≥ 0,
 *            * a(n) = Σ_{m=0}^{5} B[m] * C(n – m), * where C(R) is the number
 *            of non‑negative integer solutions of * 3·p + 4·q = R. * * C(R)
 *            can be computed directly: * qmax = ⌊R / 4⌋ * r0 =...
 * Range:     u8 n <= 203, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t B[6] = {2, 1, 5, 3, 3, 1};
    seq_t sum = 0;

    for (size_t m = 0; m < 6U; ++m)
    {
        if (n < m)
            continue;                /* R = n – m would be negative */

        idx_t R    = n - m;          /* non‑negative */
        idx_t qmax = R / 4;          /* floor(R/4)   */
        idx_t r0   = R % 3;          /* R mod 3      */

        seq_t cnt;
        if (qmax < r0)
            cnt = 0;
        else
            cnt = (seq_t)((qmax - r0) / 3 + 1);

        sum += cnt * B[m];
    }

    return sum;
}
