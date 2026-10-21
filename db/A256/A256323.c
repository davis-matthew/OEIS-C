/*
 * A256323  a(n) = numerator of (1/n^3)*(-1/(n+1) + 16/(n+2) + 3/(4*(2*n+1)) -
 *          81/(4*(2*n+3))), term of a BBP-type series representation of
 *          zeta(3) by V. Adamchik and S. Wagon.
 * https://oeis.org/A256323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of (1/n^3)*(-1/(n+1)+16/(n+2)+3/(4*(2*n+1)) -
 *            81/(4*(2*n+3)))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 20, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"


static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    return (a / gcd_seq(a, b)) * b;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range, just a safety guard */

    seq_t N  = (seq_t)n;
    seq_t d1 = N + 1;                     /* n+1 */
    seq_t d2 = N + 2;                     /* n+2 */
    seq_t d3 = 4 * ((seq_t)2 * N + 1);    /* 4*(2*n+1) */
    seq_t d4 = 4 * ((seq_t)2 * N + 3);    /* 4*(2*n+3) */

    /* LCM of the four denominators */
    seq_t D = lcm_seq(lcm_seq(lcm_seq(d1, d2), d3), d4);

    /* Sum of positive terms */
    seq_t pos = (seq_t)16 * (D / d2) + (seq_t)3 * (D / d3);

    /* Sum of negative terms */
    seq_t neg = (seq_t)1  * (D / d1) + (seq_t)81 * (D / d4);

    seq_t numerator = pos - neg;
    seq_t N3 = N * N * N;                 /* n^3 */
    seq_t denominator = D * N3;

    seq_t g = gcd_seq(numerator, denominator);
    return numerator / g;
}
