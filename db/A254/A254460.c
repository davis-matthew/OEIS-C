/*
 * A254460  a(n) is the number of predecessors of the all-ones state of the
 *          binary cellular automaton on the n X n grid graph with edges
 *          joining diagonal neighbors as well as vertical and horizontal
 *          neighbors, whose local rule is f(i,j) = sum of the state at vertex
 *          (i,j) and the states at all of its neighbor...
 * https://oeis.org/A254460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n mod 3 = 0 or 1, otherwise a(n) = 2^(2n-1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 16, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"   /* provides seq_t pow_uint(uint64_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* n >= 1 by specification */
    const idx_t r = n % 3;
    if (r == 0 || r == 1) {
        return (seq_t)1;
    }
    /* r == 2: compute 2^(2n-1) */
    idx_t exp = 2 * n - 1;
    return pow_uint((uint64_t)2, exp);
}
