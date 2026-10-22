/*
 * A366985  a(n) = Sum_{k=1..n} binomial(k+3,3) * floor(n/k).
 * https://oeis.org/A366985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(k+3,3) * floor(n/k)
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* binomial(k+3,3) = (k+3)(k+2)(k+1)/6  */
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute binomial(k+3,3) */
        seq_t binom = ((seq_t)(k + 3) * (k + 2) * (k + 1)) / 6;
        /* floor(n/k) is integer division */
        seq_t fq = binom * (seq_t)(n / k);
        sum += fq;
    }
    return sum;
}
