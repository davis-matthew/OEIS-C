/*
 * A296855  Domination number of the n-cube-connected cycle graph.
 * https://oeis.org/A296855
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n * 2^(n-2) for n ≠ 5, a(5) = 46 (n ≥ 3)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 5) return (seq_t)46;
    return (seq_t)n * pow2(n - 2);
}
