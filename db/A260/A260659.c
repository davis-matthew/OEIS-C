/*
 * A260659  Denominators of a BBP-like formula for 4*Pi/sqrt(27).
 * https://oeis.org/A260659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator((-1/8)^n*(2/(3*n+1)+1/(3*n+2)))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   gcd, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n) {
    /* compute numerator: 9*n + 5 */
    seq_t num = (seq_t)(9 * n + 5);

    /* denominator part: 8^n */
    seq_t denom = pow_int((seq_t)8, n);

    /* multiply by (3*n + 1) and (3*n + 2) */
    denom = denom * (seq_t)(3 * n + 1);
    denom = denom * (seq_t)(3 * n + 2);

    /* cancel common factors with the numerator */
    seq_t g = gcd(denom, num);
    return denom / g;
}
