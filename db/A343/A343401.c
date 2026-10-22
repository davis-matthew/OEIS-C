/*
 * A343401  Number of partitions of n into 3 parts [r,s,t] such that r < s < t
 *          where no part divides any larger part and at least 1 part divides
 *          n.
 * https://oeis.org/A343401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of partitions of n into 3 distinct parts r<s<t such
 *            that no part divides any larger part (i.e. r∤s, r∤t, s∤t) and at
 *            least one part divides n
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    idx_t r, s, t;

    /* r can be at most n/3 */
    for (r = 1; r <= n / 3; ++r)
    {
        /* s must be greater than r and such that t = n‑r‑s > s */
        idx_t max_s = (n - r - 1) / 2;      /* floor((n‑r‑1)/2) ensures t > s */
        for (s = r + 1; s <= max_s; ++s)
        {
            t = n - r - s;                  /* t > s by construction */

            /* check divisibility constraints */
            if ((s % r == 0) || (t % r == 0) || (t % s == 0))
                continue;   /* forbidden: a part divides a larger part */

            if ((n % r != 0) && (n % s != 0) && (n % t != 0))
                continue;   /* require at least one part dividing n */

            ++cnt;
        }
    }
    return cnt;
}
