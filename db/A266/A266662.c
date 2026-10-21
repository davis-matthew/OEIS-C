/*
 * A266662  Number of ON (black) cells in the n-th iteration of the "Rule 47"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-2*(-1)^n*n + 2*n + 3*(-1)^n + 1)/2 for n > 1; special
 *            cases: a(0) = 1, a(1) = 2
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Use signed 64‑bit arithmetic – n will never be large enough to overflow */
    int64_t in_n = (int64_t)n;
    int64_t parity = (n & 1ULL) ? -1 : 1;           /* (-1)^n */
    int64_t numerator = -2LL * parity * in_n
                      + 2LL * in_n
                      + 3LL * parity
                      + 1;
    return (seq_t)(numerator / 2);
}
