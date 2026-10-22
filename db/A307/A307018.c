/*
 * A307018  Total number of parts of size 3 in the partitions of n into parts
 *          of size 2 and 3.
 * https://oeis.org/A307018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* * OEIS A307018 - Total number of parts of size 3 in the partitions of n into parts 2 and 3
 *
 * Formula:  a(n) = Σ y ,   over all non‑negative integer solutions (x, y) of 2·x + 3·y = n
 *            = Σ y ,   for y ≡ n (mod 2) and 0 ≤ y ≤ ⌊n/3⌋
 *
 * The implementation evaluates the arithmetic progression of admissible y’s
 * without any floating‑point calculations and works for every n ≥ 0.
 *
 * Complexity:   O(1) arithmetic for each call
 * Memory usage:  O(1)
 */

#include <stddef.h>   /* for idx_t and seq_t */

seq_t a(idx_t n)
{
    /* first admissible y has the same parity as n */
    const idx_t first = n & 1u;          /* n mod 2  (0 for even, 1 for odd) */
    const idx_t max_y = n / 3u;          /* largest y such that 3·y ≤ n */

    /* If there is no admissible y return 0 immediately */
    if (max_y < first)
        return (seq_t)0;

    /* last admissible y : the largest y ≤ max_y with parity == first */
    const idx_t last = ((max_y & 1u) ^ first) ? (max_y - 1u) : max_y;

    /* Number of terms in the progression first, first+2, … , last */
    const idx_t count = ((last - first) / 2u) + 1u;

    /* Sum of the arithmetic progression:
     *   sum = count * (first + last) / 2
     * All intermediate computations are done in seq_t to avoid overflow
     * for large seq_t values. */
    seq_t sum = (seq_t)count * ((seq_t)first + (seq_t)last);
    sum /= 2u;

    return sum;
}
