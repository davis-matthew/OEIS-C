/*
 * A281229  Smallest number k of the form x^2 + y^2 with 0 <= x <= y such that
 *          gcd(x, y) = 1, x + y = n, and k has no other decompositions into a
 *          sum of two squares.
 * https://oeis.org/A281229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k = x^2 + y^2 with x+y=n, gcd(x,y)=1, * x<=y,
 *            and k has a unique representation (prime if n odd, * 2·prime if
 *            n even). * Implementation scans x from floor(n/2) downwards,
 *            which gives k * values in decreasing order, thus the first valid
 *            pair yields the * minimal k. * Complexity: O(...
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    idx_t half = n / 2;
    bool odd = (n & 1) != 0;

    for (idx_t i = half; ; --i)
    {
        idx_t x = i;
        idx_t y = n - x;           /* y >= x because i <= half */

        if (gcd((seq_t)x, (seq_t)y) != 1)
            continue;

        seq_t s = (seq_t)x * (seq_t)x + (seq_t)y * (seq_t)y;

        if (odd)
        {
            if (isPrimeGeneric(s))
                return s;
        }
        else
        {
            if ((s & (seq_t)1) != 0)   /* s should be even */
                continue;
            seq_t q = s >> 1;         /* q = s / 2 */
            if ((q % 4) != 1)
                continue;
            if (isPrimeGeneric(q))
                return s;             /* s = 2*q */
        }

        if (i == 0)
            break;                     /* should not happen for n>=3 */
    }

    /* Conjecture guarantees existence; return 0 as a fallback. */
    return (seq_t)0;
}
