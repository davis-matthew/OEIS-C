/*
 * A277954  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 14", based on the 5-celled von
 *          Neumann neighborhood.
 * https://oeis.org/A277954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=3, a(2)=6;
   a(n)=a(n-1)+4*a(n-2)-4*a(n-3)   for n>2 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;

    seq_t a0 = (seq_t)1;   /* a(n-3)   */
    seq_t a1 = (seq_t)3;   /* a(n-2)   */
    seq_t a2 = (seq_t)6;   /* a(n-1)   */

    for (idx_t i = 3; i <= n; ++i) {
        /* Safe guard against unsigned wrap‑around if n is max_uint64_t */
        if (i == 0) break;

        seq_t cur = a2 + 4 * a1 - 4 * a0;  /* a(n)   */
        a0 = a1;
        a1 = a2;
        a2 = cur;
    }
    return a2;   /* a(n) */
}
