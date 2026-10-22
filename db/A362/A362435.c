/*
 * A362435  a(1) = 18; thereafter a(n) = a(n-1) + abs(difference between first
 *          two digits of a(n-1)).
 * https://oeis.org/A362435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   first_two_diff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/first_two_diff.h"
/* Recurrence: a(1) = 18; for n>1
   a(n) = a(n-1) + | difference between the first two decimal digits of a(n-1) | */

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;   /* Undefined for n<1, but return 0 as safety */
    seq_t val = (seq_t)18;
    if (n == (idx_t)1)
        return val;
    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        seq_t d = first_two_diff(val);
        val += d;
    }
    return val;
}
