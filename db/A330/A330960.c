/*
 * A330960  For any n >= 0: consider all pairs of numbers (x, y) whose binary
 *          representations can be interleaved (or shuffled) to produce the
 *          binary representation of n (possibly with leading zeros); a(n) is
 *          the least possible value of x + y.
 * https://oeis.org/A330960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A330960: a(n) = minimum possible value of x + y such that the binary
 * representation of n can be obtained by interleaving the binary
 * representations of x and y (preserving order within each number and
 * allowing leading zeros).  The algorithm enumerates all subsets of the
 * binary digits of n and selects the partition that gives the smallest
 * sum x + y.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Determine the number of bits in n (0 has length 0). */
    idx_t temp = n;
    idx_t L = 0;
    while (temp) {
        temp >>= 1;
        ++L;
    }

    /* Special case n == 0 */
    if (L == 0)
        return (seq_t)0;

    /* Precompute shift masks for positions up to L-1. */
    idx_t shifts[65];
    for (idx_t i = 0; i < L; ++i)
        shifts[i] = ((idx_t)1 << i);

    /* Enumerate all ways to split the bits between x and y. */
    seq_t best = 0;
    bool first = true;
    idx_t maxmask = shifts[L - 1] * 2 - 1;          /* (1 << L) - 1  */
    for (idx_t mask = 0; mask <= maxmask; ++mask) {
        seq_t x = 0;
        seq_t y = 0;

        /* Build x and y by traversing the original bit order
         * from most significant to least significant. */
        for (idx_t pos = 0; pos < L; ++pos) {
            idx_t i = L - 1 - pos;                /* index of current bit */
            seq_t bit = (seq_t)((n >> i) & 1);
            if (mask & shifts[i]) {
                x = (x << 1) | bit;
            } else {
                y = (y << 1) | bit;
            }
        }

        seq_t sum = x + y;
        if (first || sum < best) {
            best = sum;
            first = false;
        }
    }
    return best;
}
