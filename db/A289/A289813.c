/*
 * A289813  A binary encoding of the ones in ternary representation of n (see
 *          Comments for precise definition).
 * https://oeis.org/A289813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 6560, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   recur
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0;
   a(3*n)   = 2*a(n);
   a(3*n+1) = 2*a(n)+1;
   a(3*n+2) = 2*a(n); */

#include <stdint.h>
#include "../utils/recur.h"

/* Public function required by the build. */
seq_t a(idx_t n)
{
    return recur(n);
}
