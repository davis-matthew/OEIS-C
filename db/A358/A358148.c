/*
 * A358148  Aliquot sequence starting at 326.
 * https://oeis.org/A358148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   aliquot_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A358148: aliquot sequence starting at 326
   Recurrence: a(n+1) = A001065(a(n))  (sum of proper divisors of a(n))
   Initial value: a(0) = 326                */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include "../utils/aliquot_sum.h"

/* Main sequence function.  Computes a(n) by iterating from a(0). */
seq_t a(idx_t n)
{
    seq_t cur = (seq_t)326;    /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        cur = aliquot_sum(cur);
        if (cur == (seq_t)0)   /* once we hit 0, all further terms are 0 */
            break;
    }
    return cur;
}
