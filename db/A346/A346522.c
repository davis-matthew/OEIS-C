/*
 * A346522  a(n) is the smallest number such that there are precisely n
 *          squares between a(n) and 2*a(n) inclusive.
 * https://oeis.org/A346522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling(((floor(2*n-1+sqrt(2)*(n-1)))^2)/2)
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* k = n-1 */
    seq_t k = (seq_t)(n - 1);
    /* v = 2*k*k */
    seq_t v = 2 * k * k;
    /* u = floor(sqrt(v)) = floor(k*sqrt(2)) */
    seq_t u = floor_sqrt(v);
    /* s = (2*n-1) + u */
    seq_t s = (2 * (seq_t)n - 1) + u;
    /* t = s^2 */
    seq_t t = s * s;
    /* a = ceil(t/2) = (t+1)/2 if t odd, else t/2 */
    seq_t a = (t + 1) / 2;
    return a;
}
