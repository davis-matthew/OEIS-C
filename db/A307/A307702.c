/*
 * A307702  Clique covering number of the n-Sierpinski tetrahedron graph.
 * https://oeis.org/A307702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*4^(n-3)+1 for n ≥ 3; a(1) = 1, a(2) = 4
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following utility function is provided by the build environment
   (see ../utils/pow_uint.h).  It returns a_seq_t pow_uint(uint64_t base,
   idx_t exp). */
#include "../utils/pow_uint.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    } else if (n == 2) {
        return (seq_t)4;
    } else {
        /* n ≥ 3 */
        /* Compute 4^(n-3) using the provided pow_uint utility. */
        seq_t val = pow_uint(4, n - 3);
        /* a(n) = 10 * 4^(n-3) + 1 */
        val = val * (seq_t)10 + (seq_t)1;
        return val;
    }
}
