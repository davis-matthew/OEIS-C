/*
 * A250550  Numerator of the harmonic mean of the first n 10-gonal numbers.
 * https://oeis.org/A250550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n / (∑_{k=1..n} 1/(k(4k-3)))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* 10‑gonal number: P_k = k(4k-3).  */
seq_t a(idx_t n) {
    seq_t sum_num = 0;   /* numerator of the running sum of reciprocals */
    seq_t sum_den = 1;   /* denominator of the running sum */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t ks = (seq_t)k;
        seq_t term_den = ks * ((seq_t)4 * ks - 3);   /* k*(4k-3) */

        /* add 1/term_den to sum_num/sum_den */
        seq_t new_num = sum_num * term_den + sum_den;
        seq_t new_den = sum_den * term_den;
        seq_t g = gcd(new_num, new_den);
        sum_num = new_num / g;
        sum_den = new_den / g;
    }

    /* harmonic mean H = n / (sum_num/sum_den) = n*sum_den / sum_num */
    seq_t numerator_H = (seq_t)n * sum_den;
    seq_t denom_H     = sum_num;
    seq_t g2 = gcd(numerator_H, denom_H);
    return numerator_H / g2;   /* numerator in reduced form */
}
