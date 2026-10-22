/*
 * A305057  -5x + 1 sequence starting at 5.
 * https://oeis.org/A305057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 30, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A305057:
   a(0) = 5
   a(n) = (-5) * a(n‑1) + 1   if a(n‑1) is odd
   a(n) = a(n‑1) / 2          otherwise
*/

#include <stdint.h>

/* compute the nth term of the sequence, for any idx_t (uint64_t) and any seq_t
 * (8/16/32/64-bit signed or arbitrary precision class).  The function is
 * purely iterative, so it works with all required integer representations.
 */
seq_t a(idx_t n)
{
    seq_t cur = (seq_t)5;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        if (cur & (seq_t)1)        /* odd test */
            cur = ((seq_t)-5) * cur + (seq_t)1;
        else
            cur = cur / (seq_t)2;
    }
    return cur;
}
