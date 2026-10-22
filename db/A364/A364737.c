/*
 * A364737  G.f. satisfies A(x) = 1 + x*A(x) / (1 + x*A(x)^4).
 * https://oeis.org/A364737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/n) * Sum_{k=0..n-1} (-1)^k * binomial(n,k) * *
 *            binomial(n+3*k, n-1-k), a(0) = 1
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function uses the library routine binomial(idx_t n, idx_t k)
 *  that returns a value of type seq_t.  Only 64-bit arithmetic
 *  operators are required on seq_t; no floating point or external
 *  libraries are used.  The algorithm has O(n) time complexity. */
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k < n; ++k) {
        seq_t term1 = binomial(n, k);
        seq_t term2 = binomial(n + 3 * k, n - 1 - k);
        seq_t prod = term1 * term2;
        /* (-1)^k factor */
        if ((k & 1) == 0) {
            sum += prod;
        } else {
            sum -= prod;
        }
    }
    seq_t nseq = (seq_t)n;
    return sum / nseq;
}
