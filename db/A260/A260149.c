/*
 * A260149  Expansion of f(q) * phi(q) in powers of q where f() is a Ramanujan
 *          theta function and phi() is a 6th-order mock theta function.
 * https://oeis.org/A260149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 + 2 * Σ_{m≥1} Σ_{j≥0} (-1)^j * if n = m*(3m+1)/2 + 3m*j
 *            or n = m*(3m+5)/2 + 3m*j * This counts each pair of terms with
 *            k=±m from the generating * function Sum_{k∈ℤ} x^(k*(3k+1)/2) *
 *            (1 + x^(2k))/(1 + x^(3k))
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t ans = 0;
    for (idx_t m = 1;; ++m)
    {
        /* base exponents for k = +m and k = -m */
        seq_t base1 = (seq_t)m * (((seq_t)3 * m) + 1) / 2; /* m*(3m+1)/2 */
        if (base1 > (seq_t)n)
            break; /* base2 > base1, so no further terms can contribute */

        /* first family: n = m*(3m+1)/2 + 3m*j */
        if (n >= (idx_t)base1)
        {
            idx_t diff = n - (idx_t)base1;
            idx_t step = 3 * m;
            if (diff % step == 0)
            {
                idx_t j = diff / step;
                seq_t contrib = (j & 1) ? (seq_t)(-1) : (seq_t)1;
                ans += (seq_t)2 * contrib;
            }
        }

        /* second family: n = m*(3m+5)/2 + 3m*j */
        seq_t base2 = (seq_t)m * (((seq_t)3 * m) + 5) / 2; /* m*(3m+5)/2 */
        if (n >= (idx_t)base2)
        {
            idx_t diff = n - (idx_t)base2;
            idx_t step = 3 * m;
            if (diff % step == 0)
            {
                idx_t j = diff / step;
                seq_t contrib = (j & 1) ? (seq_t)(-1) : (seq_t)1;
                ans += (seq_t)2 * contrib;
            }
        }
    }

    return ans;
}
