/*
 * A375245  Number of biquadratefree numbers <= n.
 * https://oeis.org/A375245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d>=1} mu(d) * floor(n / d^4)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Compute the Mobius function µ(d) for small d (d may be up to n^(1/4)).   */
/* It is implemented by simple prime factorisation – sufficient for the   */
/* size of d that appears in this sequence.                                 */
static int mobius(idx_t d)
{
    if (d <= 1)
        return 1;                 /* µ(1) = 1 */

    int cnt = 0;                 /* number of distinct prime factors   */
    idx_t n = d;
    for (idx_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            int mult = 0;
            while (n % p == 0)
            {
                n /= p;
                ++mult;
                if (mult > 1)
                    return 0;     /* p^2 divides d  → µ(d)=0            */
            }
            ++cnt;
        }
    }
    if (n > 1)          /* remaining prime factor larger than sqrt(d) */
        ++cnt;

    return (cnt & 1) ? -1 : 1;      /* odd → -1, even → +1             */
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* sequence starts at a(1) */

    seq_t nn = (seq_t)n;

    /* Compute floor of the fourth root: d_max = ⌊n^{1/4}⌋ */
    seq_t sqrt1 = floor_sqrt(nn);
    seq_t sqrt2 = floor_sqrt(sqrt1);
    idx_t dmax = (idx_t)sqrt2;

    seq_t sum = 0;

    for (idx_t d = 1; d <= dmax; ++d)
    {
        int mu = mobius(d);
        if (mu == 0)
            continue;

        seq_t dd = (seq_t)d;
        seq_t d2 = dd * dd;     /* d²      */
        seq_t d4 = d2 * d2;     /* d⁴      */

        seq_t term = nn / d4;   /* floor(n / d⁴) */

        if (mu > 0)
            sum += term;
        else
            sum -= term;
    }

    return sum;
}
