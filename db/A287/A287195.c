/*
 * A287195  Independence and clique covering number of the n-triangular
 *          honeycomb acute knight graph.
 * https://oeis.org/A287195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(n^2 + n + (n^2 - 5*n)*t - (6*n - 9)*t^2 + 9*t^3),
 *            where t = floor(n/3)
 * Range:     u8 n <= 37, u16 n <= 625, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert to a signed 64‑bit integer for intermediate arithmetic */
    int64_t nn = (int64_t)n;
    int64_t t  = nn / 3;               /* floor(n/3) */

    int64_t term1 = nn*nn + nn;                            /* n^2 + n */
    int64_t term2 = (nn*nn - 5*nn) * t;                    /* (n^2-5n)*t */
    int64_t t2    = t * t;                                 /* t^2    */
    int64_t term3 = (6*nn - 9) * t2;                       /* (6n-9)*t^2 */
    int64_t term4 = 9 * t * t2;                            /* 9*t^3   */

    int64_t sum = term1 + term2 - term3 + term4;           /* total numerator */
    return (seq_t)(sum / 2);                               /* integer division by 2 */
}
