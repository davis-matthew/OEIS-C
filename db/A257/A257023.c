/*
 * A257023  Number of terms in the quarter-sum representation of n.
 * https://oeis.org/A257023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = minimum number of distinct quarter squares 
 *               (numbers of the form ⌊k²/4⌋) whose sum equals n.
 * The algorithm uses a greedy subtraction of the largest possible
 * quarter square at each step, which is optimal for this set of
 * squares (the “four‑quarter‑square” theorem guarantees the result
 * uses at most four terms).
 */

#include "../utils/floor_isqrt_u64.h"

static idx_t largest_quarter_square_le(idx_t x)
{
    /* largest integer k with k² ≤ 4·x + 3 */
    idx_t k = floor_isqrt_u64(4ULL * x + 3ULL);
    /* compute ⌊k²/4⌋ without overflow */
    unsigned __int128 prod = (unsigned __int128)k * k;
    return (idx_t)(prod >> 2);          /* prod / 4 */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;        /* a(0) = 1 by definition */

    idx_t remaining = n;
    seq_t  count    = 0;

    while (remaining > 0) {
        idx_t q = largest_quarter_square_le(remaining);
        remaining -= q;
        ++count;
    }

    return count;
}
