/*
 * A362289  a(n) is the largest denominator when the greedy algorithm for
 *          Egyptian fractions is applied to 1/n + 1/(n+1).
 * https://oeis.org/A362289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A050210(n*(n+1), 2*n+1)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 21, u64 n <= 31, u128 n <= 36,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;                /* safeguard index type */
    /* initial fraction (2*n+1)/(n*(n+1)) */
    seq_t num = 2*nt + 1;
    seq_t den = nt * (nt + 1);

    seq_t max_denom = 0;

    while (num != 0) {
        /* next unit fraction denominator (ceil(den/num)) */
        seq_t d = den / num;
        if (den % num != 0)
            d++;

        if (d > max_denom)
            max_denom = d;

        /* subtract 1/d:   num/den - 1/d = (num*d - den)/(den*d)   */
        seq_t new_num = num * d - den;
        seq_t new_den = den * d;

        if (new_num == 0)       /* last term reached */
            break;

        /* reduce the fraction */
        seq_t g = gcd(new_num, new_den);
        if (g > 1) {
            new_num /= g;
            new_den /= g;
        }

        num = new_num;
        den = new_den;
    }

    return max_denom;
}
