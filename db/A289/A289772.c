/*
 * A289772  a(n) is the numerator of b(n) where b(n) = 1/(3*(1+2*A112765(n) -
 *          b(n-1))) and b(0) = 0, where A112765(n) is the 5-adic valuation of
 *          n.
 * https://oeis.org/A289772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, tabf, look
 * Formula:   not stated
 * Range:     u8 n <= 335, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the numerator of the rational b(n) defined by
 *    b(0) = 0,
 *    b(n) = 1 / (3 * ((1 + 2*v_5(n)) - b(n‑1))) ,
 *  where v_5(n) is the 5‑adic valuation of n (the exponent of the highest
 *  power of 5 dividing n).  The returned value is reduced to lowest
 *  terms; only the numerator is returned.
 */
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* b(0) = 0/1 -> numerator is 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t p = (seq_t)0;   /* numerator of b(k-1) */
    seq_t q = (seq_t)1;   /* denominator of b(k-1) */

    for (idx_t k = 1; k <= n; ++k) {
        /* 5‑adic valuation of k */
        unsigned v = 0;
        idx_t t = k;
        while ((t % 5) == 0) {
            ++v;
            t /= 5;
        }

        /* (1 + 2*v) as seq_t */
        seq_t vterm = (seq_t)1 + (seq_t)2 * v;

        /* N = (1+2*v)*q - p  */
        seq_t N = vterm * q - p;

        /* b(k) = q / (3*N)  ->   numerator = q, denominator = 3*N */
        seq_t p_next = q;
        seq_t q_next = (seq_t)3 * N;

        /* Reduce fraction */
        seq_t g = gcd(p_next, q_next);
        p_next /= g;
        q_next /= g;

        /* By construction q_next is positive; the previous argument
           guarantees N > 0 for all k, hence q_next > 0.  Nevertheless
           guard against accidental zero values. */
        if (q_next == (seq_t)0) {
            /* sequence is undefined beyond this point; return 0. */
            return (seq_t)0;
        }

        p = p_next;
        q = q_next;
    }

    return p;   /* numerator of b(n) */
}
