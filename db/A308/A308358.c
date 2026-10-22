/*
 * A308358  Beatty sequence for sqrt(3)/4.
 * https://oeis.org/A308358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(3)/4)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute floor(n*sqrt(3)/4).  
       Since floor(n*sqrt(3)/4) = floor(floor(n*sqrt(3))/4) and
       floor(n*sqrt(3)) = floor(sqrt(3*n*n)), we first find the
       integer part of sqrt(3*n*n) and then divide by 4. */
    seq_t nn   = (seq_t)n;
    seq_t sq   = nn * nn;          /* n^2      */
    sq        = sq * (seq_t)3;     /* 3*n^2    */
    seq_t root = floor_sqrt(sq);   /* floor(sqrt(3*n^2)) */
    return root / 4;
}
