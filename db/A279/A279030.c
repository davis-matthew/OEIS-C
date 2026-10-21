/*
 * A279030  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 129", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A279030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + (-1)^n)*(2^(2 + n)-1)/6
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n & 1) return (seq_t)0;
    seq_t val = pow2(n + 2);
    val -= (seq_t)1;
    val /= (seq_t)3;
    return val;
}
