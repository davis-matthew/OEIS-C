/*
 * A390529  Weighted count of equivalence classes of words w of length n+1
 *          over {a, b, c, d, A, *} having exactly n letters, exactly one *,
 *          an even number of lowercase a's, and weight (-1)^(A(w)), where
 *          A(w) is the number of uppercase letters A.
 * https://oeis.org/A390529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(1 + 2^(n+1)) - (3^(n+1) + 1)/2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    /* Compute 2^n, 2^(2n+1) and 3^(n+1) */
    seq_t two_n      = pow2(n);                 /* 2^n */
    seq_t two_2n1    = pow2((n << 1) + 1);      /* 2^(2n+1) */
    seq_t three_np1  = pow3(n + 1);             /* 3^(n+1) */

    /* a(n) = (2^(2n+1) + 2^n) - ((3^(n+1) + 1) / 2) */
    seq_t sum = two_2n1 + two_n;                /* 2^(2n+1) + 2^n */
    seq_t tmp = three_np1 + 1;
    tmp >>= 1;                                 /* divide by 2 */

    return sum - tmp;
}
