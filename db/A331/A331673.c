/*
 * A331673  Sum of all base-n numbers with digit sum n and length exactly n.
 * https://oeis.org/A331673
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of all base‑n numbers with digit sum n and length
 *            exactly n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   binom, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;                 /* undefined for n<2, return 0 */
    /* binomial coefficient C(2n-2, n-1) */
    seq_t C = binom(2*n-2, n-1);
    seq_t N = C - 1;                      /* number of valid strings */

    /* sum of least significant digits (position 0, most significant digit) */
    seq_t t = (C * (seq_t)(2*n-1)) / (seq_t)n;
    seq_t sum_d0 = t - (seq_t)n;          /* S0 = C*(2n-1)/n - n */

    /* sum of each other digit (positions 1..n-1) */
    seq_t S1 = (((seq_t)n) * N - sum_d0) / (seq_t)(n-1);

    /* n^(n-1) */
    seq_t pow_val = pow_seq((seq_t)n, n-1);

    /* contribution of most significant digit */
    seq_t contrib0 = sum_d0 * pow_val;

    /* geometric sum (n^(n-1)-1)/(n-1) */
    seq_t G = (pow_val - 1) / (seq_t)(n-1);

    /* final result */
    return contrib0 + S1 * G;
}
