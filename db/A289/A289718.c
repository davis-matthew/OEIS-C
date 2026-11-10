/*
 * A289718  The order of the semigroup of orientation-preserving or reserving
 *          full transformations of n elements.
 * https://oeis.org/A289718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A005430(n) - n*A006004(n), where A005430(n) = n*C(2n,n)
 *            and A006004(n) = (n^3 - 2n^2 + 5n - 2)/2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-08)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t A005430_a(idx_t n) {
    if (n == 0) return (seq_t)0;
    return (seq_t)n * binomial(2 * n, n);
}

static seq_t A006004_a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    return (n3 - 2 * n2 + 5 * n_seq - 2) / 2;
}

seq_t a(idx_t n) {
    return A005430_a(n) - (seq_t)n * A006004_a(n);
}
