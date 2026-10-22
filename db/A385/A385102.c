/*
 * A385102  Consecutive internal states of the linear congruential
 *          pseudo-random number generator for Turbo Pascal 3.0 when started
 *          at 1.
 * https://oeis.org/A385102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = (129*a(n-1)+907633385) mod 2^32
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base case: a(1) = 1 */
    seq_t cur = 1;
    if (n <= 1) return cur;

    /* Modulus 2^32 mask */
    const seq_t mask = (seq_t)0xffffffffu;

    /* Iterate using the recurrence */
    for (idx_t i = 2; i <= n; ++i)
    {
        cur = ((seq_t)129 * cur + (seq_t)907633385u) & mask;
    }

    return cur;
}
