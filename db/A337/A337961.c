/*
 * A337961  Number of chiral pairs of colorings of the 12 pentagonal faces of
 *          a regular dodecahedron or the 12 vertices of a regular icosahedron
 *          using n or fewer colors.
 * https://oeis.org/A337961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) * n^2 * (n+1) * (n^8 + n^6 - 14*n^4 + 44) / 120
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 60, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;
    seq_t n6 = n4 * n2;
    seq_t n8 = n4 * n4;
    seq_t P  = n8 + n6 - 14 * n4 + 44;

    /* Parts of the numerator */
    seq_t parts[5];
    parts[0] = nn - 1;  /* (n-1) */
    parts[1] = nn;      /* n */
    parts[2] = nn;      /* n */
    parts[3] = nn + 1;  /* (n+1) */
    parts[4] = P;       /* (n^8 + n^6 - 14*n^4 + 44) */

    seq_t result = 1;
    idx_t denom   = 120;  /* denominator */

    for (int i = 0; i < 5; ++i) {
        seq_t g = gcd(parts[i], (seq_t)denom);
        if (g != 0) {
            parts[i] /= g;
            denom   /= (idx_t)g;
        }
        result *= parts[i];
    }

    if (denom > 1) {
        result /= (seq_t)denom;
    }

    return result;
}
