/*
 * A259459  From higher-order arithmetic progressions.
 * https://oeis.org/A259459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+3)! * (n+2)! / ( 2^(n+2) * (n+1) * (n+2) * 6 )
 *            Recurrence: a(0)=1, a(n) = ((n+3)*(n+2)^2)/(2*n) * a(n−1) for
 *            n≥1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Compute a(i) from a(i-1) = res */
        seq_t aprev = res;

        /* Denominator factors: i and 2 */
        seq_t d1 = (seq_t)i;
        seq_t d2 = (seq_t)2;

        /* Reduce with aprev */
        seq_t g = gcd(aprev, d1);
        aprev /= g;
        d1 /= g;

        g = gcd(aprev, d2);
        aprev /= g;
        d2 /= g;

        /* Multiply by numerator factors (i+3)*(i+2)*(i+2) */
        seq_t num1 = (seq_t)(i + 3);
        seq_t num2 = (seq_t)(i + 2);

        aprev *= num1;
        aprev *= num2;
        aprev *= num2;

        /* Finish reduction with remaining denominators */
        if (d1 != (seq_t)1)
            aprev /= d1;
        if (d2 != (seq_t)1)
            aprev /= d2;

        res = aprev;
    }
    return res;
}
