/*
 * A334991  a(n) = 4^n + 3 * 18^n.
 * https://oeis.org/A334991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A334991
   Formula: a(n) = 4^n + 3 * 18^n.
   Recurrence: a(0) = 4, a(1) = 58,
               a(n) = 22*a(n-1) - 72*a(n-2)   for n > 1
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0u)
        return (seq_t)4;
    if (n == 1u)
        return (seq_t)58;

    /* Iteratively compute using the linear recurrence */
    seq_t a0 = (seq_t)4;   /* a(0) */
    seq_t a1 = (seq_t)58;  /* a(1) */
    seq_t ai;
    idx_t i;

    for (i = 2; i <= n; ++i) {
        ai = a1 * (seq_t)22 - a0 * (seq_t)72;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
