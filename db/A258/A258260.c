/*
 * A258260  Expansion of f(x) + 2*f(x^2) + 3*f(x^9) + 6*f(x^18) in powers of x
 *          where f(x) := x / (1 + x^2).
 * https://oeis.org/A258260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = sum_{(m,w)∈{(1,1),(2,2),(9,3),(18,6)}} w * (-1)^k where n
 *            = m*(2k+1) is a valid representation; otherwise the term
 *            contributes 0. This follows from expanding
 *            f(x)+2f(x^2)+3f(x^9)+6f(x^18) with f(x)=x/(1+x^2)=∑_{k≥0} (-1)^k
 *            x^{2k+1}
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 */

    seq_t result = 0;

    /* pairs (m,w) representing the four summands:
       f(x)      -> m=1,  w=1
       2f(x^2)   -> m=2,  w=2
       3f(x^9)   -> m=9,  w=3
       6f(x^18)  -> m=18, w=6 */
    const struct { idx_t m; seq_t w; } pw[] = {
        { 1,  1 },
        { 2,  2 },
        { 9,  3 },
        { 18, 6 }
    };
    const size_t len = sizeof(pw) / sizeof(pw[0]);

    for (size_t i = 0; i < len; ++i) {
        idx_t m = pw[i].m;
        if (n % m != 0) continue;          /* term does not contribute */

        idx_t t = n / m;                    /* t = 2k+1 must be odd */
        if ((t & 1) == 0) continue;         /* t even → impossible */

        idx_t k = (t - 1) >> 1;             /* k = (t-1)/2       */
        seq_t sign = ((k & 1) != 0) ? -1 : 1;/* (-1)^k */

        result += sign * pw[i].w;
    }

    return result;
}
