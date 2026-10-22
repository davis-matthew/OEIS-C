/*
 * A317083  a(n) is the first term less than the initial 2n+1 in the reduced
 *          Collatz trajectory.
 * https://oeis.org/A317083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the first Collatz trajectory term less than the initial
 *            odd * 2n+1, where the trajectory uses the reduced Collatz map: *
 *            x → (3x+1)/2^k with k chosen so that the result is odd
 * Range:     u8 n <= 12, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   reduced_collatz
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reduced_collatz.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                /* a(0) = 1 by definition */

    seq_t start = (seq_t)n * 2 + 1;   /* initial odd value 2n+1 */
    seq_t m = start;

    while (1) {
        m = reduced_collatz(m);
        if (m < start)
            return m;
    }
}
