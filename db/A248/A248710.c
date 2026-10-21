/*
 * A248710  a(n) = f(6*n+3)/(f(n-2)*f(n-1)*f(n)*f(n+1)*f(n+2)*f(n+3)), where
 *          f(k) = k!.
 * https://oeis.org/A248710
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6n+3)! / ((n-2)!(n-1)!n!(n+1)!(n+2)!(n+3)!) (multinomial
 *            coefficient with parts n-2, n-1, n, n+1, n+2, n+3) Sequence
 *            offset 2: a(2) is the first term listed
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Below the offset behaviour is unspecified. */
    if (n < 2)
        return (seq_t)0;

    /* a(2) is known. For any higher n we compute iteratively from a(2). */
    if (n == 2)
        return (seq_t)37837800;

    seq_t val = (seq_t)37837800;          /* a(2) */

    /* Iterate from a(3) up to a(n) using the recurrence:
       a(k) = a(k-1) * Π_{t=4}^{9} (6(k-1)+t) / Π_{t=-1}^{4} (k-1+t)   */
    for (idx_t k = 3; k <= n; ++k)
    {
        idx_t prev = k - 1;
        seq_t numer[6];
        seq_t denom[6];

        /* Numerators: 6*(k-1)+4 ... 6*(k-1)+9 */
        for (int t = 0; t < 6; ++t)
            numer[t] = (seq_t)(6 * prev + 4 + t);

        /* Denominators: (k-1)-1 ... (k-1)+4  i.e., k-2 ... k+3 */
        for (int t = 0; t < 6; ++t)
            denom[t] = (seq_t)(prev - 1 + t);

        /* Cancel common factors between val and the denominators. */
        for (int d = 0; d < 6; ++d)
        {
            if (denom[d] == 0)        /* should not occur, guard anyway */
                continue;
            seq_t g = gcd(val, denom[d]);
            if (g > 1)
            {
                val   /= g;
                denom[d] /= g;
            }
        }

        /* Cancel remaining denominators with the numerators. */
        for (int d = 0; d < 6; ++d)
        {
            if (denom[d] == 1)
                continue;
            for (int m = 0; m < 6; ++m)
            {
                if (numer[m] == 1)
                    continue;
                seq_t g = gcd(denom[d], numer[m]);
                if (g > 1)
                {
                    denom[d] /= g;
                    numer[m] /= g;
                    if (denom[d] == 1)
                        break;
                }
            }
        }

        /* Multiply the remaining numerators. */
        for (int m = 0; m < 6; ++m)
            if (numer[m] != 1)
                val *= numer[m];

        /* Divide any remaining denominators (they should all be 1). */
        for (int d = 0; d < 6; ++d)
            if (denom[d] != 1)
                val /= denom[d];
    }

    return val;
}
