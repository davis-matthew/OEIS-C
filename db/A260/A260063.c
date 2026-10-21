/*
 * A260063  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000001 00000011 or 00000111.
 * https://oeis.org/A260063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 25, u64 n <= 54, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A260063:  a(n) = a(n-1)+4*a(n-3)+5*a(n-4)-5*a(n-5)-2*a(n-6)
   Initial values:
   a(1)=48   a(2)=91   a(3)=256   a(4)=557   a(5)=956   a(6)=2131 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Undefined for n = 0, but return 0 to keep signature clean. */
    if (n == 0) return (seq_t)0;

    /* Base cases.  The sequence is 1‑indexed. */
    if (n == 1) return (seq_t)48;
    if (n == 2) return (seq_t)91;
    if (n == 3) return (seq_t)256;
    if (n == 4) return (seq_t)557;
    if (n == 5) return (seq_t)956;
    if (n == 6) return (seq_t)2131;

    /* Maintain the six most recent terms of the recurrence. */
    seq_t a1 = (seq_t)48;    /* a(n-6)   */
    seq_t a2 = (seq_t)91;    /* a(n-5)   */
    seq_t a3 = (seq_t)256;   /* a(n-4)   */
    seq_t a4 = (seq_t)557;   /* a(n-3)   */
    seq_t a5 = (seq_t)956;   /* a(n-2)   */
    seq_t a6 = (seq_t)2131;  /* a(n-1)   */

    for (idx_t i = 7; i <= n; ++i)
    {
        /* Compute the next term using the recurrence. */
        seq_t an = a6
                 + (seq_t)4 * a4
                 + (seq_t)5 * a3
                 - (seq_t)5 * a2
                 - (seq_t)2 * a1;

        /* Shift the window of recent terms. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = an;
    }

    return a6;
}
