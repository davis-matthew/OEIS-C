/*
 * A242305  Squares-greedy residue of n.
 * https://oeis.org/A242305
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n minus the sum of distinct perfect squares less than n
 *            chosen greedily from the largest to the smallest. That is, let m
 *            = ⌊√(n – 1)⌋. For k = m, m – 1, … , 1 if k² ≤ current_residue
 *            subtract k² from it. The final residue is a(n). (This implements
 *            the squares‑greedy residue of n.)
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square root (floor) for 64‑bit unsigned integers. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, mid, res = 0;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        /* mid*mid cannot overflow 64 bits because mid ≤ √n ≤ 2³²‑1. */
        if ((unsigned __int128)mid * mid <= n) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                    /* offset starts at 2 */

    /* All squares used must be strictly less than n. */
    idx_t max_k = isqrt_u64(n - 1);          /* floor(sqrt(n-1)) */

    idx_t residue = n;
    for (idx_t k = max_k; k >= 1; --k) {
        idx_t sq = k * k;
        if (sq <= residue) {
            residue -= sq;
            /* square k is now used; next iterations consider only smaller k. */
        }
    }
    return (seq_t)residue;
}
