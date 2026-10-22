/*
 * A346596  Let m = A344005(n) = smallest m such that n divides m*(m+1); a(n)
 *          = max(gcd(n,m), gcd(n,m+1)).
 * https://oeis.org/A346596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let m = least m such that n divides m*(m+1). Then a(n) =
 *            max(gcd(n,m), gcd(n,m+1))
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;          /* a(1) = 1 */

    for (idx_t m = 1; m < n; ++m) {
        seq_t g1 = gcd((seq_t)n, (seq_t)m);          /* gcd(n, m)   */
        seq_t g2 = gcd((seq_t)n, (seq_t)(m + 1));    /* gcd(n, m+1) */

        /* Condition: n | m*(m+1) ⇔ g1 * g2 == n.
           We avoid overflow by checking g2 == n / g1. */
        if ((idx_t)g2 == (idx_t)n / (idx_t)g1) {
            return g1 > g2 ? g1 : g2;              /* max(g1,g2) */
        }
    }

    /* The loop should always find a solution for n > 1. */
    return (seq_t)n;
}
