/*
 * A260656  a(n) = lcm{!2, !3, ..., !n}, where !n = A000166(n) is
 *          subfactorial.
 * https://oeis.org/A260656
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm{!k : 2 <= k <= n} where !k is the subfactorial of k
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset starts at 2; values below 2 are irrelevant */

    seq_t lcm = 1;                /* current least common multiple */
    seq_t prev2 = 1;              /* !0 = 1 */
    seq_t prev1 = 0;              /* !1 = 0 */

    for (idx_t k = 2; k <= n; ++k)
    {
        /* Subfactorial recurrence: !k = (k‑1)*( !(k‑1) + !(k‑2) ) */
        seq_t sub = (seq_t)(k - 1) * (prev1 + prev2);

        /* Update the running lcm: lcm = lcm / gcd(lcm, sub) * sub */
        seq_t g = gcd(lcm, sub);
        lcm = (lcm / g) * sub;

        /* Shift the recurrence window */
        prev2 = prev1;
        prev1 = sub;
    }

    return lcm;
}
