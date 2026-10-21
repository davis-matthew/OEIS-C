/*
 * A291775  Domination number of the n-Sierpinski carpet graph.
 * https://oeis.org/A291775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1) = 3; for n>1 : a(n) = 2^(3n-2) + 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */
    if (n == 1) return 3;          /* special initial case */
    return pow2(3 * n - 2) + 2;    /* 2^(3n-2)+2 for n>1 */
}
