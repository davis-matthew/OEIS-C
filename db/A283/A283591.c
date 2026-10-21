/*
 * A283591  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 657", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A283591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=5, a(n)=2*(2^n-1) for n>2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)5;
    seq_t p = pow2(n);          /* compute 2^n */
    return (seq_t)2 * (p - (seq_t)1);
}
