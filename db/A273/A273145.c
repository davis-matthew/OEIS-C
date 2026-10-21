/*
 * A273145  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 597", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 1, a(1) = 8, a(n) = 14*4^(n-1) - 4*2^n + 1 for n > 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    /* 4^(n-1) = 2^(2*(n-1)) */
    idx_t exp4 = 2 * (n - 1);
    seq_t pow4 = pow_int((seq_t)2, exp4);         /* 4^(n-1) */

    seq_t term1 = pow4 * (seq_t)14;               /* 14*4^(n-1) */

    seq_t pow2n = pow_int((seq_t)2, n);           /* 2^n */
    seq_t term2 = pow2n * (seq_t)4;               /* 4*2^n */

    return term1 - term2 + (seq_t)1;
}
