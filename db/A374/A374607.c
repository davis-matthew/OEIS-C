/*
 * A374607  a(n) is the numerator of (1134*n^3 + 2097*n^2 + 1188*n +
 *          193)/(10368*n^4 + 20736*n^3 + 14112*n^2 + 3744*n + 320).
 * https://oeis.org/A374607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of (1134*n^3 + 2097*n^2 + 1188*n + 193) /
 *            (10368*n^4 + 20736*n^3 + 14112*n^2 + 3744*n + 320) after
 *            reducing the fraction to lowest terms
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 155, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t tn  = (seq_t)n;
    seq_t t2  = tn * tn;
    seq_t t3  = t2 * tn;

    /* numerator of the rational function */
    seq_t num = (seq_t)1134 * t3 + (seq_t)2097 * t2 + (seq_t)1188 * tn + (seq_t)193;

    /* compute denominator modulo num to avoid overflow */
    seq_t den_mod = 0;

    /* term 10368*n^4 */
    {
        seq_t pow_mod = 1;
        for (idx_t i = 0; i < 4; ++i) pow_mod = (pow_mod * tn) % num;
        seq_t term = ((seq_t)10368 * pow_mod) % num;
        den_mod = (den_mod + term) % num;
    }
    /* term 20736*n^3 */
    {
        seq_t pow_mod = 1;
        for (idx_t i = 0; i < 3; ++i) pow_mod = (pow_mod * tn) % num;
        seq_t term = ((seq_t)20736 * pow_mod) % num;
        den_mod = (den_mod + term) % num;
    }
    /* term 14112*n^2 */
    {
        seq_t pow_mod = 1;
        for (idx_t i = 0; i < 2; ++i) pow_mod = (pow_mod * tn) % num;
        seq_t term = ((seq_t)14112 * pow_mod) % num;
        den_mod = (den_mod + term) % num;
    }
    /* term 3744*n */
    {
        seq_t pow_mod = tn % num;
        seq_t term = ((seq_t)3744 * pow_mod) % num;
        den_mod = (den_mod + term) % num;
    }
    /* constant term 320 */
    {
        seq_t term = (seq_t)320 % num;
        den_mod = (den_mod + term) % num;
    }

    /* reduce numerator by the gcd of numerator and denominator */
    seq_t g = gcd(num, den_mod);
    return num / g;
}
