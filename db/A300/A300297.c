/*
 * A300297  Denominators of (1/8)*n*(5 + 3*n)/((1 + 3*n)*(4 + 3*n)), n >= 0.
 * https://oeis.org/A300297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of (1/8)*n*(5+3*n)/((1+3*n)*(4+3*n))
 * Range:     u8 n <= 1, u16 n <= 29, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Convert index to seq_t first to avoid overflow mixed with idx_t. */
    seq_t nn = (seq_t)n;

    /* Compute numerator and denominator of the rational value. */
    seq_t p = nn * ((seq_t)5 + (seq_t)3 * nn);
    seq_t q = (seq_t)8 * ((seq_t)1 + (seq_t)3 * nn) *
             ((seq_t)4 + (seq_t)3 * nn);

    /* Reduce fraction: denominator after simplification is q / gcd(p,q). */
    seq_t g = gcd(p, q);

    return q / g;
}
