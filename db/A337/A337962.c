/*
 * A337962  Number of achiral colorings of the 12 pentagonal faces of a
 *          regular dodecahedron or the 12 vertices of a regular icosahedron
 *          using n or fewer colors.
 * https://oeis.org/A337962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (15*n^6 + n^4 + 44) / 60 (OEIS A337962)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 19, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;          /* cast index to seq_t */
    seq_t n2 = N * N;             /* n^2 */
    seq_t n4 = n2 * n2;           /* n^4 */
    seq_t n6 = n4 * n2;           /* n^6 */

    /* B = 15*n^6 + n^4 + 44 */
    seq_t B = 15 * n6 + n4 + 44;

    /* reduce the factor 60 before multiplying to avoid overflow */
    seq_t div = (seq_t)60;

    /* first divide by gcd(n^2, div) */
    seq_t g1 = gcd(n2, div);
    seq_t A  = n2 / g1;
    div /= g1;

    /* then divide by gcd(B, div) */
    seq_t g2 = gcd(B, div);
    B /= g2;
    div /= g2;

    /* any remaining divisor divides B, so divide it */
    if (div != 1)
        B /= div;

    /* final result */
    return A * B;
}
