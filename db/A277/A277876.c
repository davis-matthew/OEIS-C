/*
 * A277876  a(n) = n!/(m*(n-m)) with m = floor(n/2).
 * https://oeis.org/A277876
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! / (floor(n/2) * (n - floor(n/2)))
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* uses the gcd function from utils */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* floor(n/2) */
    idx_t m = n / 2;
    seq_t d1 = (seq_t)m;                    /* divisor part 1: m   */
    seq_t d2 = (seq_t)(n - m);              /* divisor part 2: n-m */

    seq_t result = 1;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* cancel parts of the denominator that may divide the current factor */
        seq_t x = (seq_t)i;

        if (d1 > 1)
        {
            seq_t g = gcd(x, d1);
            if (g > 1)
            {
                x   /= g;
                d1  /= g;
            }
        }
        if (d2 > 1)
        {
            seq_t g = gcd(x, d2);
            if (g > 1)
            {
                x   /= g;
                d2  /= g;
            }
        }

        /* multiply the (possibly reduced) factor into the running product */
        result *= x;

        /* now reduce the product itself by any remaining denominator parts */
        if (d1 > 1)
        {
            seq_t g = gcd(result, d1);
            if (g > 1)
            {
                result /= g;
                d1     /= g;
            }
        }
        if (d2 > 1)
        {
            seq_t g = gcd(result, d2);
            if (g > 1)
            {
                result /= g;
                d2     /= g;
            }
        }
    }

    /* any small remaining factors of the denominator should be absorbed now */
    if (d1 > 1)
        result /= d1;
    if (d2 > 1)
        result /= d2;

    return result;
}
