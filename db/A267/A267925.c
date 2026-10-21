/*
 * A267925  Binary representation of the n-th iteration of the "Rule 246"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 10000*a(n-2) - 10000*a(n-3) for n>2, a(0) = 1,
 *            a(1) = 111, a(2) = 10111
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)111;
    if (n == 2) return (seq_t)10111;

    seq_t a0 = (seq_t)1;          /* a(n-3)  */
    seq_t a1 = (seq_t)111;        /* a(n-2)  */
    seq_t a2 = (seq_t)10111;      /* a(n-1)  */

    seq_t ai;
    for (idx_t i = 3; i <= n; ++i) {
        ai = a2 + (seq_t)10000 * a1 - (seq_t)10000 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
