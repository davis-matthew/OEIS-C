/*
 * A259458  From higher-order arithmetic progressions.
 * https://oeis.org/A259458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+4)!*(n+1)*(n+2)*(n+3)^2*(n+4)^2 / 384
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(0)=18;  a(n) = (n+4)^3/(n*(n+2)) * a(n‑1) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)18;

    seq_t res = (seq_t)18;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* (k+4)^3 */
        seq_t term = (seq_t)k + (seq_t)4;
        seq_t num  = term * term * term;

        /* k*(k+2) */
        seq_t den  = (seq_t)k * ((seq_t)k + (seq_t)2);

        /* Reduce numerator and denominator */
        seq_t g = gcd(num, den);
        if (g != (seq_t)1) {
            num /= g;
            den /= g;
        }

        /* Reduce res and denominator */
        g = gcd(res, den);
        if (g != (seq_t)1) {
            res /= g;
            den /= g;
        }

        /* Multiply and divide */
        res = res * num;
        if (den != (seq_t)1)
            res = res / den;
    }

    return res;
}
