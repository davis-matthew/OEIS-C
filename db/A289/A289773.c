/*
 * A289773  a(n) is the denominator of b(n) where b(n) = 1/(3*(1+2*A112765(n)
 *          - b(n-1))) and b(0) = 0, with A112765(n) being the 5-adic
 *          valuation of n.
 * https://oeis.org/A289773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, tabf, look
 * Formula:   a(n) is the denominator of b(n) defined by b(0) = 0, b(n) = 1 /
 *            (3 * (1 + 2 * v5(n) - b(n-1)) ) (n > 0), where v5(n) is the
 *            5‑adic valuation of n. The implementation evaluates b(n) as a
 *            reduced fraction and returns its denominator
 * Range:     u8 n <= 779, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* b(0) = 0/1  – the initial fraction */
    seq_t p = 0;   /* numerator of b(k-1) */
    seq_t q = 1;   /* denominator of b(k-1) */

    if (n == 0)
        return (seq_t)1;   /* denominator of b(0) */

    for (idx_t i = 1; i <= n; ++i) {
        /* compute 5‑adic valuation of i */
        idx_t t = i, v = 0;
        while (t % 5 == 0) { ++v; t /= 5; }

        /* compute new numerator and denominator before reduction */
        seq_t mult     = (seq_t)1 + (seq_t)2 * (seq_t)v;
        seq_t rhs      = q * mult - p;        /*  q*(1+2v) - p  */
        seq_t new_q    = (seq_t)3 * rhs;      /* 3 * rhs        */
        seq_t new_p    = q;                   /* previous q     */

        /* reduce the fraction */
        seq_t g = gcd(new_p, new_q);
        new_p /= g;
        new_q /= g;

        /* prepare for next iteration */
        p = new_p;
        q = new_q;
    }

    return q;   /* denominator of b(n) */
}
