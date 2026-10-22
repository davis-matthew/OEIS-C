/*
 * A393714  a(n) = number of triples (x, y, z) such that x^2 + y*z = n, where
 *          x, y, z are positive integers and x is coprime to y*z.
 * https://oeis.org/A393714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x,y,z) with x^2 + y*z = n, x, y, z > 0
 *            and gcd(x, y*z) = 1. For each x, let t = n - x^2. If gcd(x,t)=1,
 *            the number of ordered pairs (y,z) with y*z = t equals the number
 *            of divisors of t. Sum this over all x such that x^2 <= n
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t total = 0;

    /* iterate x such that x^2 <= n */
    for (idx_t x = 1; (idx_t)x * x <= n; ++x)
    {
        idx_t t = n - (idx_t)x * x;      /* y*z must equal t */
        if (t == 0) continue;            /* no positive y,z for t = 0 */

        /* gcd(x, y*z) == gcd(x, t) */
        if (gcd((seq_t)x, (seq_t)t) == 1)
        {
            seq_t cnt = 0;
            for (idx_t d = 1; d * d <= t; ++d)
            {
                if (t % d == 0)
                {
                    if (d * d == t)       /* square root contributes 1 pair */
                        cnt += 1;
                    else                  /* d and t/d give two ordered pairs */
                        cnt += 2;
                }
            }
            total += cnt;
        }
    }

    return total;
}
