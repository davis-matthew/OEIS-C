/*
 * A250549  Denominator of the harmonic mean of the first n 9-gonal numbers.
 * https://oeis.org/A250549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the harmonic mean of the first n 9‑gonal
 *            numbers. 9‑gonal numbers: P(k) = (7k² – 5k)/2. We accumulate S =
 *            Σ_{k=1..n} 1/P(k) as a reduced fraction S = a/b. Then the
 *            harmonic mean H = n / S = n·b / a. The reduced denominator of H
 *            is a / gcd(n·b, a)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Sum of reciprocals S = a/b */
    seq_t a_num = 0;   /* numerator of S */
    seq_t b_den = 1;   /* denominator of S */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;

        /* denom of 1/P(k) :   2 / (k·(7k – 5))  */
        seq_t denom_term = k_seq * (((seq_t)7 * k_seq) - (seq_t)5); /* k*(7k-5) */
        seq_t num_term   = (seq_t)2;                               /* 2 */

        /* Reduce the addition with a common denominator */
        seq_t g_den = gcd(b_den, denom_term);              /* gcd(b_den, denom_term) */
        seq_t b_div_g = b_den / g_den;
        seq_t d_div_g = denom_term / g_den;

        /* New denominator: lcm(b_den, denom_term) = b_div_g * denom_term */
        seq_t new_den = b_div_g * denom_term;

        /* New numerator: a_num * (denom_term/g) + num_term * (b_den/g) */
        seq_t term1 = a_num * d_div_g;
        seq_t term2 = num_term * b_div_g;
        seq_t new_num = term1 + term2;

        /* Reduce fraction */
        seq_t g = gcd(new_num, new_den);
        a_num = new_num / g;
        b_den = new_den / g;
    }

    /* Harmonic mean H = (n * b_den) / a_num  (reduced) */
    seq_t nV = ((seq_t)n) * b_den;
    seq_t d = gcd(nV, a_num);

    return a_num / d;   /* denominator of the reduced harmonic mean */
}
