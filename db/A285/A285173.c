/*
 * A285173  Numbers n such that A002496(n+1) < A002496(n)^(1+1/n).
 * https://oeis.org/A285173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   _is_excluded
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: all natural numbers except 1, 2, 4, 351.  The function returns
   the nth positive integer that is not equal to 1, 2, 4 or 351.  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_is_excluded.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t x = 1;
    while (true) {
        if (!_is_excluded(x)) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
        ++x;
    }
}
