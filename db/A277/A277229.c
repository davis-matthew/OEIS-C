/*
 * A277229  Convolution of the odd-indexed triangular numbers (A000384(n+1))
 *          and the squares (A000290).
 * https://oeis.org/A277229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(4*n^2 + 3*n + 3)/60
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 144, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;

    /* binomial part : n*(n+1)*(n+2)/6 */
    seq_t val = nn * ((seq_t)(n + 1));
    val /= 2;                   /* divide by 2 */
    val *= ((seq_t)(n + 2));
    val /= 3;                   /* divide by 3, now val = binomial(n+2,3) */

    /* extra part : 4*n^2 + 3*n + 3 */
    seq_t extra = ((seq_t)4) * nn * nn + ((seq_t)3) * nn + 3;

    /* cancel the factor 10 : a(n) = binomial * extra / 10 */
    seq_t g = gcd(val, (seq_t)10);
    val /= g;
    seq_t denom = 10 / g;

    seq_t g2 = gcd(extra, denom);
    extra /= g2;
    denom /= g2;

    /* denom should now be 1 */
    seq_t result = val * extra;
    if (denom > 1) result /= denom;   /* defensive, normally never happens */

    return result;
}
