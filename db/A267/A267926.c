/*
 * A267926  Decimal representation of the n-th iteration of the "Rule 246"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+16*a(n-2)-16*a(n-3); initial values: a(0)=1,
 *            a(1)=7, a(2)=23
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)7;
    if (n == 2u) return (seq_t)23;

    seq_t a0 = (seq_t)1;   /* a(n-3)  */
    seq_t a1 = (seq_t)7;   /* a(n-2)  */
    seq_t a2 = (seq_t)23;  /* a(n-1)  */
    seq_t an;

    for (idx_t i = 3u; i <= n; ++i)
    {
        an = a2 + a1 * 16u - a0 * 16u;  /* recurrence */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
