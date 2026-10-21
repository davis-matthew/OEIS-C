/*
 * A244280  Denominators of the n-th iteration of the alternating continued
 *          fraction of the positive integers, initiated with (1 + ...).
 * https://oeis.org/A244280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the alternating continued fraction a(n) =
 *            1/(1+1/(2-1/(3+1/(4-...1/(n +/- 1))))). The returned value is
 *            the numerator of the intermediate fraction value(1), which is
 *            the denominator of the whole expression
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Value at the deepest level */
    seq_t p = ( (n & 1ULL) ? (seq_t)n + 1 : (seq_t)n - 1 );  /* n +/- 1 */
    seq_t q = 1;

    /* Work backwards from n-1 down to 1 */
    for (idx_t i = n - 1; i; --i)
    {
        /* i +/- 1/value(i+1) */
        if (i & 1ULL)   /* odd: plus */
        {
            seq_t num = (seq_t)i * p + q;  /* (i * p + q) / p */
            seq_t den = p;
            seq_t g = gcd(num, den);
            if (g > 1)
            {
                num /= g;
                den /= g;
            }
            p = num;
            q = den;
        }
        else            /* even: minus */
        {
            seq_t num = (seq_t)i * p - q; /* (i * p - q) / p */
            seq_t den = p;
            seq_t g = gcd(num, den);
            if (g > 1)
            {
                num /= g;
                den /= g;
            }
            p = num;
            q = den;
        }
    }

    /* The overall fraction is 1/(p/q) = q/p, so the denominator is p */
    return p;
}
