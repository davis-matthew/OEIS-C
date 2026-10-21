/*
 * A286184  Number of connected induced (non-null) subgraphs of the helm graph
 *          with 2n+1 nodes.
 * https://oeis.org/A286184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + (1 + n)*2^n - n
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & (idx_t)1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* The sequence is defined for n ≥ 1; we still handle n = 0 safely. */
    seq_t two_pow   = pow_seq((seq_t)2, n);          /* 2^n */
    seq_t three_pow = pow_seq((seq_t)3, n);          /* 3^n */
    seq_t term2     = two_pow * ((seq_t)(n + (idx_t)1)); /* (1+n)*2^n */
    return three_pow + term2 - (seq_t)n;             /* a(n) */
}
