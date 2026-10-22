/*
 * A362545  Number of odd chordless cycles of length >4 in the (2n+1)-flower
 *          snark.
 * https://oeis.org/A362545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=13, a(2)=81, a(n)=7*a(n-1)-7*a(n-2)+a(n-3) for n≥3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)81;

    seq_t a0 = (seq_t)1;   /* a(n-3)   */
    seq_t a1 = (seq_t)13;  /* a(n-2)   */
    seq_t a2 = (seq_t)81;  /* a(n-1)   */

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 7*a(i-1) - 7*a(i-2) + a(i-3) */
        seq_t next = 7 * a2 - 7 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
