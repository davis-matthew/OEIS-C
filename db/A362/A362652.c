/*
 * A362652  Expansion of g.f. x*(-2 - 2*x + x^2 - x^3)/((1 + x)^2 *(-1 +
 *          x)^3).
 * https://oeis.org/A362652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + (n mod 2) + (n * (n - (n mod 2))) / 2
 * Range:     u8 n <= 21, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t mod2 = (seq_t)(n & 1);                 /* n mod 2 */
    seq_t term = ((seq_t)n * ((seq_t)n - mod2)) >> 1; /* (n*(n-mod2))/2 */
    return (seq_t)n + mod2 + term;
}
