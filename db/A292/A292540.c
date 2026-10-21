/*
 * A292540  Number of 3-cycles in the n-Sierpinski tetrahedron graph.
 * https://oeis.org/A292540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*4^(n-1) for n>1, a(1)=4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;          /* not part of the sequence offset, but safe */
    }
    if (n == 1) {
        return (seq_t)4;
    }

    /* Compute 5 * 4^(n-1) */
    seq_t result = (seq_t)5;
    for (idx_t i = 1; i < n; ++i) {
        result *= (seq_t)4;
    }
    return result;
}
