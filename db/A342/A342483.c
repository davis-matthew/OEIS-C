/*
 * A342483  a(n) = binomial(n,2)*(2^(n-2) - n).
 * https://oeis.org/A342483
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,2)*(2^(n-2)-n)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 25, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* For n < 2 the formula is undefined; we return 0. */
    if (n < 2) return 0;

    seq_t nseq = (seq_t)n;

    /* Compute binomial(n,2) without overflow */
    seq_t binom;
    if ((nseq & 1) == 0) {
        binom = (nseq / 2) * (nseq - 1);
    } else {
        binom = nseq * ((nseq - 1) / 2);
    }

    /* 2^(n-2) */
    seq_t pow_val = pow2(n - 2);

    /* 2^(n-2) - n */
    seq_t term = pow_val - nseq;

    /* Final result */
    return binom * term;
}
