/*
 * A372832  a(n) is the numerator of Sum_{d|n, d <= sqrt(n)} 1/d.
 * https://oeis.org/A372832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Σ_{d|n, d ≤ √n} 1/d
 * Range:     u8 n <= 139, u16 n <= 9381, u32 n <= 9381, u64 n <= 9381,
 *            u128 n <= 9381, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* Compute the numerator of the sum of reciprocals of all divisors of n
 * that are ≤ floor(sqrt(n)).  The sum is expressed as a reduced fraction,
 * and only the numerator is returned. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1, but return 0 for safety */

    /* Integer floor square root of n by a linear scan (n ≤ ~1e4 in practice). */
    idx_t sqrt_n = 0;
    while ((sqrt_n + 1) * (sqrt_n + 1) <= n)
        ++sqrt_n;

    seq_t num = 0;          /* current numerator   */
    seq_t den = 1;          /* current denominator */

    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d != 0)
            continue;      /* not a divisor */

        seq_t d_seq = (seq_t)d;

        /* Add the fraction 1/d to the current sum num/den */
        seq_t g = gcd(den, d_seq);
        seq_t lcm = (den / g) * d_seq;          /* lcm of den and d_seq */
        seq_t term_num = den / g;               /* numerator of 1/d over lcm */
        seq_t new_num = num * (d_seq / g) + term_num;
        seq_t new_den = lcm;
        seq_t g2 = gcd(new_num, new_den);       /* reduce the fraction */

        num = new_num / g2;
        den = new_den / g2;
    }

    return num;   /* the reduced numerator of the sum */
}
