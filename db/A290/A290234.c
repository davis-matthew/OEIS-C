/*
 * A290234  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 773", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) - 1 for n even a(n) = 2^(n+1) - 2^((n+1)/2) for n
 *            odd (OEIS A290234)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    idx_t exp = n + 1;                  /* exponent for 2^...      */
    seq_t pow2_exp = pow2(exp);         /* 2^(n+1)                 */
    if ((n & 1) == 0) {                /* even n */
        return pow2_exp - (seq_t)1;
    } else {                           /* odd n */
        seq_t sub = pow2(exp / 2);     /* 2^((n+1)/2)             */
        return pow2_exp - sub;
    }
}
