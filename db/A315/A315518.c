/*
 * A315518  Coordination sequence Gal.5.291.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   diff_is_six
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A315518 – Coordination sequence Gal.5.291.4
   a(0) = 1
   For n>0 the increment from a(n-1) to a(n) is 5 or 6.
   The increment is 6 exactly when
       n % 5 == 3
       or     n % 5 == 0  and  (n/5) % 2 == 0
       or     n % 5 == 1  and  (n/5) % 2 == 0  and  n/5 >= 1
   Otherwise the increment is 5.
   This implements the exact definition of the sequence.
*/

#include <stdbool.h>
#include "../utils/diff_is_six.h"

seq_t a(idx_t n)
{
    seq_t sum = 1;   /* a(0) */
    if (n == 0) return sum;

    for (idx_t i = 1; i <= n; ++i) {
        sum += (diff_is_six(i) ? 6 : 5);
    }
    return sum;
}
