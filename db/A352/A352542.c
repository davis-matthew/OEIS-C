/*
 * A352542  Trajectory of initial value 89 under iterations of the map
 *          A352544: half if even, add largest anagram if odd.
 * https://oeis.org/A352542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 31, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   largest_anagram
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 89;
   a(n+1) = a(n)/2          if a(n) is even
            a(n)+largest_anagram(a(n))  if a(n) is odd   */
/* largest_anagram returns the largest decimal anagram of its argument. */

#include <stdlib.h>   /* for NULL */
#include "../utils/largest_anagram.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)89;
    seq_t val = (seq_t)89;
    for (idx_t i = 1; i <= n; ++i) {
        if (val & 1) {              /* odd */
            val = val + largest_anagram(val);
        } else {                    /* even */
            val = val / 2;
        }
    }
    return val;
}
