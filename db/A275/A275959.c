/*
 * A275959  Sum of distinct terms of A002674: a(0) = 0, a(2n) =
 *          A255411(A153880(a(n))), a(2n+1) = 1+A255411(A153880(a(n))).
 * https://oeis.org/A275959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i>=0, bit i of n set} A002674(i+1), where A002674(k)
 *            = (2k)!/2
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 63, u64 n <= 1023,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 58 terms through n = 8183 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A002674_term(idx_t k) {
    seq_t result = 1;
    seq_t n_cast = (seq_t)k;
    seq_t n_sq = n_cast * n_cast;
    for (idx_t j = 0; j < k; j++) {
        seq_t j_cast = (seq_t)j;
        result *= (n_sq - j_cast * j_cast);
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t term = 1; /* A002674(1) = 1 */
    idx_t k = 1;
    (void)A002674_term;
    while (n > 0) {
        if (n & 1) sum += term;
        n >>= 1;
        /* (2(k+1))!/2 = (2k)!/2 * (2k+1)(2k+2) */
        term *= (seq_t)(2 * k + 1) * (seq_t)(2 * k + 2);
        k++;
    }
    return sum;
}
