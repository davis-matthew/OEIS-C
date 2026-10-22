/*
 * A361035  a(n) = 9979200 * (4*n)!/(n!*(n+3)!^3).
 * https://oeis.org/A361035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9979200 * (4*n)!/(n!*(n+3)!^3) Recurrence: a(0) = 46200;
 *            a(n) = (4*(4*n-1)*(4*n-2)*(4*n-3)/(n+3)^3) * a(n-1) (for n ≥ 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"

/* Multiply *res by *t, canceling any common factors with *D before and after
   the multiplication.  The updated remaining denominator is stored back in *D_ptr.
   All operations are performed on the exact integer type seq_t, so the result
   is correct as soon as the final division by *D_ptr is performed.
*/
static seq_t mul_and_cancel(seq_t res, seq_t t, seq_t *D_ptr)
{
    seq_t D = *D_ptr;

    /* cancel any common factor between the term t and the remaining denominator */
    seq_t g = gcd(t, D);
    if (g > 1) {
        t /= g;
        D /= g;
    }

    /* cancel any factor from res that can reduce the remaining denominator */
    g = gcd(res, D);
    if (g > 1) {
        res /= g;
        D /= g;
    }

    /* multiply the term */
    res = res * t;

    /* cancel again with the (now possibly reduced) denominator */
    g = gcd(res, D);
    if (g > 1) {
        res /= g;
        D /= g;
    }

    *D_ptr = D;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)46200;

    seq_t prev = (seq_t)46200;

    for (idx_t i = 1; i <= n; ++i) {
        /* Remaining denominator (i+3)^3 */
        idx_t ip3 = i + 3;
        seq_t D = (seq_t)ip3;
        D = D * (seq_t)ip3 * (seq_t)ip3;   /* (i+3)^3 */

        /* Numerator terms 4, 4*i-1, 4*i-2, 4*i-3 */
        seq_t t1 = (seq_t)4;
        seq_t t2 = (seq_t)(4 * i - 1);
        seq_t t3 = (seq_t)(4 * i - 2);
        seq_t t4 = (seq_t)(4 * i - 3);

        /* Cancel any common factors of each term with the denominator
           before the multiplication phase.                               */
        seq_t g = gcd(t1, D); if (g > 1) { t1 /= g; D /= g; }
        g = gcd(t2, D); if (g > 1) { t2 /= g; D /= g; }
        g = gcd(t3, D); if (g > 1) { t3 /= g; D /= g; }
        g = gcd(t4, D); if (g > 1) { t4 /= g; D /= g; }

        seq_t res = prev;

        /* Multiply successively, cancelling factors with the denominator.       */
        res = mul_and_cancel(res, t1, &D);
        res = mul_and_cancel(res, t2, &D);
        res = mul_and_cancel(res, t3, &D);
        res = mul_and_cancel(res, t4, &D);

        /* Any remaining denominator must divide the result exactly. */
        if (D > 1)
            res = res / D;

        prev = res;
    }

    return prev;
}
