/*
 * A250400  Numerator of the harmonic mean of the first n octagonal numbers.
 * https://oeis.org/A250400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numerator of the harmonic mean of the first n octagonal
 *            numbers. Octagonal number O_k = k*(3*k-2). The harmonic mean
 *            H(n) = n / (Σ_{k=1..n} 1/O_k). Let Σ = p/q in lowest terms; then
 *            H(n) = n*q/p, and a(n) is the numerator of this reduced
 *            fraction. The implementation keeps the sum of recipr...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 21, u128 n <= 38,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Sequence is defined for n ≥ 1; for safety return 0 for n = 0. */
    if (n == 0) return 0;

    seq_t sum_num = 0;   /* numerator of Σ(1/O_k) in reduced form */
    seq_t sum_den = 1;   /* denominator of Σ(1/O_k) in reduced form */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;
        seq_t O_k   = k_seq * ( (seq_t)3 * k_seq - (seq_t)2 );

        /* Add 1/O_k to the running sum:
           new_num = sum_num*O_k + sum_den
           new_den = sum_den*O_k */
        seq_t new_num = sum_num * O_k + sum_den;
        seq_t new_den = sum_den * O_k;

        /* Reduce the fraction with gcd. */
        seq_t g = gcd(new_num, new_den);
        sum_num = new_num / g;
        sum_den = new_den / g;
    }

    /* H(n) = n / (sum_num/sum_den) = n * sum_den / sum_num. */
    seq_t numerator   = (seq_t)n * sum_den;
    seq_t denominator = sum_num;

    /* Reduce the final fraction to get a(n). */
    seq_t g_final = gcd(numerator, denominator);
    numerator /= g_final;

    return numerator;
}
