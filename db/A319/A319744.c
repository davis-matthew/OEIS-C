/*
 * A319744  Partial sums of bouncy numbers (A152054).
 * https://oeis.org/A319744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_bouncy
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial sums of bouncy numbers (A152054). a(n) = sum of first n bouncy numbers. */

#include <stdbool.h>
#include "../utils/is_bouncy.h"

/* Compute the a(n) value.  We scan forward from 100 and accumulate once
   for each bouncy number.  No dynamic allocation is used, so the code
   works for all seq_t types, including arbitrary‑precision integers. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t   count = 0;
    seq_t   sum   = 0;
    idx_t   num   = 100;   /* the first candidate is 100 */

    while (count < n) {
        if (is_bouncy(num)) {
            sum += (seq_t)num;
            ++count;
        }
        ++num;
    }
    return sum;
}
