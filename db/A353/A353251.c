/*
 * A353251  a(0) = 1, a(n) = harmonic_mean(a(n-1), n), where harmonic_mean(p,
 *          q) = 2/(1/p + 1/q); denominators.
 * https://oeis.org/A353251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(0) = 1, a(n) = denominator( 2/(1/a(n‑1) + 1/n) ) for n ≥ 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t num = (seq_t)1;   /* numerator of a(0) = 1/1 */
    seq_t den = (seq_t)1;   /* denominator of a(0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;

        /* new fraction before reduction:
           numerator = 2 * num * i
           denominator = den * i + num
        */
        seq_t new_num = (seq_t)2 * num * i_seq;
        seq_t new_den = den * i_seq + num;

        /* reduction */
        seq_t g = gcd(new_num, new_den);
        if (g != (seq_t)1) {
            new_num /= g;
            new_den /= g;
        }

        num = new_num;
        den = new_den;
    }

    return den;
}
