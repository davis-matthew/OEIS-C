/*
 * A393995  Difference between the numbers of binary words of length n with an
 *          even and with an odd number of occurrences of subword 101.
 * https://oeis.org/A393995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 50, u64 n <= 101,
 *            u128 n <= 203, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3)  for n ≥ 3
   Initial values: a(0) = 1, a(1) = 2, a(2) = 4
   This recurrence follows from the rational generating function */

#include <stdint.h>   /* for standard integer types */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;
    if (n == (idx_t)2) return (seq_t)4;

    seq_t a0 = (seq_t)1;   /* a(n-3)   */
    seq_t a1 = (seq_t)2;   /* a(n-2)   */
    seq_t a2 = (seq_t)4;   /* a(n-1)   */
    seq_t next;

    for (idx_t i = (idx_t)3; i <= n; ++i)
    {
        /* a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) */
        next = (a2 * (seq_t)2) - (a1 * (seq_t)2) + (a0 * (seq_t)2);

        /* rotate the sliding window */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;
}
