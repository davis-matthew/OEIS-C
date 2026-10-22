/*
 * A323239  a(n) = 1 if n is odd and squarefree, otherwise a(n) = 0.
 * https://oeis.org/A323239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if n is odd and squarefree, otherwise 0
 * Range:     u8 n <= 2932, u16 n <= 9983, u32 n <= 9983, u64 n <= 9983,
 *            u128 n <= 9983, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* Zero is not considered odd */
    if (n == 0) return 0;

    /* Check oddness: only odd n can be 1 */
    if (((seq_t)n & 1) == 0) return 0;

    /* Use the supplied helper to test squarefreeness */
    return is_squarefree((seq_t)n) ? 1 : 0;
}
