/*
 * A349201  a(n) = [x^n] ((x^2*(1 + 3*x + x^2 - 2*x^3 + 3*x^4 + x^5 -
 *          x^6))/((-1 + x)^4 *(1 + x)^3)).
 * https://oeis.org/A349201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((5 + 3*n - n^2)*(1 - (-1)^n) + 2*n^3)/16 for n > 1; a(1)
 *            = 0
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n = 1 dominates the offset */
    if (n <= 1)
        return (seq_t)0;

    /* Use 64‑bit signed arithmetic for the intermediate computation.
       For typical n ≤ 10 000 the values fit easily in a long long. */
    long long nn = (long long)n;
    long long part = (5 + 3 * nn - nn * nn) * ((nn & 1) ? 2 : 0);   /* 1-(-1)^n */
    long long other = 2 * nn * nn * nn;                               /* 2*n^3   */
    long long num  = part + other;                                    /* numerator */
    return (seq_t)(num / 16);                                         /* divide by 16 */
}
