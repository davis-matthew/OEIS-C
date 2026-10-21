/*
 * A296519  Denominator of n*Sum_{k=1..n} 1/(n+k).
 * https://oeis.org/A296519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of n * sum_{k = 1 .. n} 1/(n + k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Current fraction num/den, initially 0/1. */
    seq_t num = 0;
    seq_t den = 1;
    seq_t nn  = (seq_t)n;   /* promote n to seq_t for arithmetic */

    /* Sum the terms n/(n+1), n/(n+2), … , n/(2n). */
    for (idx_t j = n + 1; j <= n + n; ++j) {
        seq_t gj = gcd(nn, (seq_t)j);       /* reduce n/j */
        seq_t term_num = nn / gj;
        seq_t term_den = (seq_t)j / gj;

        seq_t g = gcd(den, term_den);      /* common factor for lcm */

        /* Calculate new numerator and denominator (lcm of den and term_den). */
        seq_t n1 = term_den / g;
        seq_t d1 = den / g;

        seq_t new_num = num * n1 + term_num * d1;
        seq_t new_den = d1 * term_den;

        /* Reduce the fraction again. */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    /* The denominator of the reduced sum is the required value. */
    return den;
}
