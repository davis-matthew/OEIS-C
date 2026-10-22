/*
 * A356012  a(n) = n! / (6 * floor(n/3)).
 * https://oeis.org/A356012
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! / (6 * floor(n/3))
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 3, but we compute for any n >= 0
       returning 0 for n < 3. */
    if (n < 3) {
        return 0;
    }

    /* denominator = 6 * floor(n/3) */
    seq_t denom = (seq_t)6 * (seq_t)(n / 3);
    seq_t result = 1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = (seq_t)i;
        seq_t g = gcd(term, denom);
        if (g > 1) {
            /* cancel the common factor between term and remaining denominator */
            denom /= g;
            term /= g;
        }
        result *= term;
    }
    /* After the loop denom must be 1, as 6*floor(n/3) divides n! */
    return result;
}
