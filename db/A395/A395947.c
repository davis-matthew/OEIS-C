/*
 * A395947  Expansion of e.g.f.: x*sinh(x)^2*cosh(x) (odd powers only).
 * https://oeis.org/A395947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1) * (3^(2*n-2)-1) / 4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* allowed for completeness */

seq_t a(idx_t n)
{
    if (n == 0)        /* offset starts at 1, but guard against 0 */
        return 0;

    /* exponent: 2*n-2 = 2*(n-1) */
    const idx_t exp = 2 * (n - 1);

    /* compute 3^exp in seq_t (wrap‑around for values beyond seq_t capacity) */
    seq_t power = 1;
    for (idx_t i = 0; i < exp; ++i)
        power *= (seq_t)3;

    /* because 3^(even) ≡ 1 (mod 4), the numerator is divisible by 4 */
    const seq_t temp  = (power - 1) / 4;
    const seq_t factor = (seq_t)(2 * n - 1);

    return temp * factor;
}
