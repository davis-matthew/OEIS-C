/*
 * A267801  Binary representation of the n-th iteration of the "Rule 213"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 101*a(n-1) - 100*a(n-2) for n > 3 a(0) = 1, a(1) = 11,
 *            a(2) = 10011, a(3) = 1110011
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)10011;
    if (n == 3) return (seq_t)1110011;

    /* Iterate using the recurrence for n >= 4. */
    seq_t prev2 = (seq_t)10011;   /* a(2) */
    seq_t prev1 = (seq_t)1110011; /* a(3) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        cur = (seq_t)101 * prev1 - (seq_t)100 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
