/*
 * A334668  Numerator of Sum_{k=1..n} (-1)^(k+1)/k^7.
 * https://oeis.org/A334668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{k=1..n} (-1)^(k+1) / k^7 (in lowest
 *            terms)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute gcd for seq_t using the helper function */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t num = (seq_t)0;   /* current numerator   */
    seq_t den = (seq_t)1;   /* current denominator */

    for (idx_t k = (idx_t)1; k <= n; ++k)
    {
        /* sign = (-1)^(k+1) */
        int sign = (k & (idx_t)1) ? 1 : -1;

        seq_t kbig = (seq_t)k;
        /* Compute k^7 = k * k^6 */
        seq_t k7 = kbig;
        for (int i = 0; i < 6; ++i) k7 *= kbig;   /* 7 multiplications */

        /* lcm of current denominator and k7 */
        seq_t g  = gcd(den, k7);
        seq_t l  = (den / g) * k7;

        /* adjust numerator to common denominator l */
        seq_t mul1 = l / den;   /* multiplier for existing num   */
        seq_t mul2 = l / k7;    /* multiplier for new term       */

        /* Add or subtract the new term */
        if (sign > 0)
            num = num * mul1 + mul2;
        else
            num = num * mul1 - mul2;

        den = l;

        /* Reduce fraction to keep numbers small */
        seq_t g2 = gcd(num, den);
        if (g2 != (seq_t)1)
        {
            num /= g2;
            den /= g2;
        }
    }

    return num;   /* numerator in lowest terms */
}
