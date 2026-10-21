/*
 * A271899  a(n) = 1 + 471*n/140 + 1594*n^2/315 + 73*n^3/16 + 161*n^4/60 +
 *          83*n^5/80 + 61*n^6/240 + n^7/28 + 11*n^8/5040.
 * https://oeis.org/A271899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (n+4)*(n+3)*(n+2)*(n+1)*(11*n^4+70*n^3+196*n^2+269*n+210)/5040
 * Range:     u8 none, u16 n <= 6, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t N  = n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;

    /* polynomial part: 11*n^4 + 70*n^3 + 196*n^2 + 269*n + 210 */
    seq_t poly  = 11;
    poly        = poly * n4;                 /* 11*n^4 */
    poly       += (seq_t)70 * n3;
    poly       += (seq_t)196 * n2;
    poly       += (seq_t)269 * N;
    poly       += (seq_t)210;

    seq_t factors[5];
    factors[0] = N + (seq_t)4;
    factors[1] = N + (seq_t)3;
    factors[2] = N + (seq_t)2;
    factors[3] = N + (seq_t)1;
    factors[4] = poly;

    seq_t denom = 5040;
    seq_t res   = 1;

    for (int i = 0; i < 5; ++i) {
        seq_t f = factors[i];

        /* cancel common part of f and denom */
        seq_t g = gcd(f, denom);
        f      /= g;
        denom  /= g;

        /* also cancel part of the running product if possible */
        g = gcd(res, denom);
        res /= g;
        denom /= g;

        /* multiply the remaining factor */
        res *= f;
    }

    /* any remaining denominator should be 1 */
    res /= denom;
    return res;
}
