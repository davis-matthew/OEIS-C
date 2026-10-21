/*
 * A282817  Number of inequivalent ways to color the faces of a cube using at
 *          most n colors so that no color appears more than twice.
 * https://oeis.org/A282817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)^2*(n-1)*n^2*(n+5)/24
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t denom = 24;
    seq_t factors[6];
    factors[0] = (seq_t)(n - 2);
    factors[1] = (seq_t)(n - 2);
    factors[2] = (seq_t)(n - 1);
    factors[3] = (seq_t)n;
    factors[4] = (seq_t)n;
    factors[5] = (seq_t)(n + 5);

    seq_t res = 1;
    for (int i = 0; i < 6; ++i) {
        seq_t f = factors[i];
        seq_t g = gcd(f, denom);
        f /= g;
        denom /= g;
        res *= f;
    }
    return res;
}
