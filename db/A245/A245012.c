/*
 * A245012  The number of labeled caterpillar graphs on n nodes.
 * https://oeis.org/A245012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{s=1}^{n-1} C(n, s) * P(s) * F(s, n-s) where P(1) =
 *            1, P(s) = s! / 2 for s >= 2 (number of path structures on the s
 *            internal vertices) F(1, r) = 1 if r >= 2, else 0 (star case)
 *            F(2, r) = 2^r - 2 if r >= 2, else 0 (two endpoints) For s >= 3:
 *            F(s, r) = s^r - 2*(s-1)^r + (s-2)^r (leaf ass...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;          /* path of two vertices */

    seq_t ans = 0;                        /* sum over s >= 1 */

    /* iterative binomial coefficients binom(n, s) */
    seq_t comb = 1;                       /* binom(n, 0) */
    /* internal path count P(s)   (s! / 2 for s >= 2, 1 for s = 1) */
    seq_t internal = 1;                   /* P(1) = 1 */

    for (idx_t s = 1; s < n; ++s)
    {
        /* update binomial for current s */
        comb = comb * (n - s + 1) / s;    /* now comb = C(n, s) */

        /* update internal path count */
        if (s > 2) internal *= s;        /* P(s) = P(s-1) * s   for s >= 3 */

        idx_t leaves = n - s;
        seq_t f = 0;

        if (s == 1)
        {
            if (leaves >= 2) f = 1;
        }
        else if (s == 2)
        {
            if (leaves >= 2) f = pow_uint(2, leaves) - 2;
        }
        else /* s >= 3 */
        {
            seq_t p1 = pow_uint(s, leaves);
            seq_t p2 = pow_uint(s - 1, leaves);
            seq_t p3 = pow_uint(s - 2, leaves);
            f = p1 - (p2 + p2) + p3;      /* s^r - 2*(s-1)^r + (s-2)^r */
        }

        if (f == 0) continue;           /* no valid trees for this s */

        /* term = C(n, s) * P(s) * F(s, n-s) */
        seq_t term = comb * internal * f;
        ans += term;
    }

    return ans;
}
