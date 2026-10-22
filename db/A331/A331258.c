/*
 * A331258  Denominator of squared radius of inscribed circle of the n-th
 *          triangle with integer sides in the list given by A316841.
 *          Numerators are A331257.
 * https://oeis.org/A331258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Function: a(n)
 * Description: Computes a(n) as the denominator of the squared radius of
 * the inscribed circle of the n-th triangle listed in OEIS A316841.
 * Triangles are enumerated in the following order:
 *   for c = 1,2,3,...
 *     for b = 1,2,…,c
 *       for a = 1,2,…,b
 *         if a + b > c then triangle (a,b,c) is valid and emitted.
 * The denominator is obtained from the reduced fraction
 *     r^2 = ((P-2a)(P-2b)(P-2c)) / (4*P)
 * where P = a+b+c.  The function returns den / gcd(num, den).
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t c = 1; ; ++c) {
        for (idx_t b = 1; b <= c; ++b) {
            for (idx_t a = 1; a <= b; ++a) {
                if (a + b <= c) continue;   /* not a triangle */
                ++count;
                if (count == n) {
                    seq_t P = (seq_t)a + (seq_t)b + (seq_t)c;
                    seq_t num = (P - (seq_t)2*a) * (P - (seq_t)2*b) * (P - (seq_t)2*c);
                    seq_t den = (seq_t)4 * P;
                    seq_t g   = gcd(num, den);
                    return den / g;
                }
            }
        }
    }
}
