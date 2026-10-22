/*
 * A353250  a(0) = 1, a(n) = harmonic_mean(a(n-1), n), where harmonic_mean(p,
 *          q) = 2/(1/p + 1/q); numerators.
 * https://oeis.org/A353250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(0) = 1; a(n) = numerator of harmonic_mean(a(n-1), n) where
 *            harmonic_mean(p,q) = 2/(1/p + 1/q)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t num = (seq_t)1;   /* numerator of a(n-1) */
    seq_t den = (seq_t)1;   /* denominator of a(n-1) */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t ii   = (seq_t)i;                     /* i as seq_t */
        seq_t nnum = (seq_t)2 * num * ii;          /* 2 * a_prev * i   */
        seq_t nden = den * ii + num;              /* a_prev_den * i + a_prev */

        seq_t g = gcd(nnum, nden);                /* reduce fraction   */
        nnum /= g;
        nden /= g;

        num = nnum;
        den = nden;
    }
    return num;
}
