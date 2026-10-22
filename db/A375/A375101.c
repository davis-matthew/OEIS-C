/*
 * A375101  a(0) = 1; a(n+1) = 10*a(n) + A010888(a(n)), where A010888 =
 *          digital root.
 * https://oeis.org/A375101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; a(n+1) = 10*a(n) + A010888(a(n)),
   where A010888(x) is the digital root of x. */

#include <stddef.h>  /* for idx_t (provided by the build) */

seq_t a(idx_t n)
{
    seq_t cur = 1;                     /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        /* digital root: 1 + (x-1) % 9  (x > 0) */
        seq_t dr = (seq_t)1 + (cur - (seq_t)1) % (seq_t)9;
        cur = cur * (seq_t)10 + dr;   /* a(n+1) */
    }
    return cur;
}
