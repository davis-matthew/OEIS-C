/*
 * A255433  a(n) = Product_{k=0..n} (k^3+1).
 * https://oeis.org/A255433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n} (k^3+1) (OEIS A255433)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence by iteratively multiplying the terms. */
#include "../utils/seq_mul.h"   /* provides seq_mul(seq_t, seq_t) */

seq_t a(idx_t n) {
    seq_t res = 1;                      /* a(0) = 1, identity for multiplication */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = (seq_t)k * k * k + 1;   /* k^3 + 1, fits in 64‑bit for all n ≤ 10000 */
        res = seq_mul(res, term);            /* multiply into the running product */
    }
    return res;
}
