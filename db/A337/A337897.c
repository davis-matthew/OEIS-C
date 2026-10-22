/*
 * A337897  Number of achiral colorings of the 8 triangular faces of a regular
 *          octahedron or the 8 vertices of a cube using n or fewer colors.
 * https://oeis.org/A337897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (7 + 2*n^2 + 3*n^4) / 12
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*               = 1*C(n,1) + 19*C(n,2) + 141*C(n,3) + 394*C(n,4)
 *                 + 450*C(n,5) + 180*C(n,6)                 */
#include "../utils/gcd.h"

static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;   /* use symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n + 1 - i);
        seq_t den = (seq_t)i;

        seq_t g = gcd(res, den);
        res /= g;
        den /= g;

        g = gcd(num, den);
        num /= g;
        den /= g;        /* den should now be 1 */

        res *= num;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t result = sn;                           /* 1*C(n,1) = n */

    seq_t c2 = binom_small(n, 2);
    result += (seq_t)19 * c2;                   /* 19*C(n,2) */

    seq_t c3 = binom_small(n, 3);
    result += (seq_t)141 * c3;                  /* 141*C(n,3) */

    seq_t c4 = binom_small(n, 4);
    result += (seq_t)394 * c4;                  /* 394*C(n,4) */

    seq_t c5 = binom_small(n, 5);
    result += (seq_t)450 * c5;                  /* 450*C(n,5) */

    seq_t c6 = binom_small(n, 6);
    result += (seq_t)180 * c6;                  /* 180*C(n,6) */

    return result;
}
