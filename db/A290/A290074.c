/*
 * A290074  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 641", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1; For n>1: - if n is even: a(n)=3*2^(n-1) - 1 - if
 *            n is odd : a(n)=2^(n-1) - 1 (Verified against OEIS A290074)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) {
        return (seq_t)1;
    }
    /* n > 1 */
    seq_t pow2_n_1 = pow2(n - 1);         /* 2^(n-1) */
    if ((n & 1) == 0) {                  /* even n */
        return pow2_n_1 * (seq_t)3 - (seq_t)1;
    } else {                              /* odd n */
        return pow2_n_1 - (seq_t)1;
    }
}
