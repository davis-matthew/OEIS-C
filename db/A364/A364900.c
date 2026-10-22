/*
 * A364900  The n-volume of the unit regular n-simplex is
 *          sqrt(a(n))/A364901(n), with a(n) being squarefree.
 * https://oeis.org/A364900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = squarefree_part( (n+1) if n even, (n+1)/2 if n odd )
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   squarefree_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/squarefree_part.h"

seq_t a(idx_t n)
{
    idx_t val;
    if ((n & 1U) == 0)      /* even n */
        val = n + 1;
    else                   /* odd n */
        val = (n + 1) / 2;
    return squarefree_part(val);
}
