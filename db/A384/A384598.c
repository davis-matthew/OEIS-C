/*
 * A384598  Expansion of (1-3*x^2) / (1-x-4*x^2+2*x^3+2*x^4).
 * https://oeis.org/A384598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 57, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 4*a(n-2) - 2*a(n-3) - 2*a(n-4)
   Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=4 */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    /* store the last four terms, a(n-4)..a(n-1) */
    seq_t a0 = (seq_t)1;  /* a(0)   */
    seq_t a1 = (seq_t)1;  /* a(1)   */
    seq_t a2 = (seq_t)2;  /* a(2)   */
    seq_t a3 = (seq_t)4;  /* a(3)   */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* compute a(i) from the recurrence */
        seq_t next = a3 + (seq_t)4 * a2 - (seq_t)2 * a1 - (seq_t)2 * a0;

        /* shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
