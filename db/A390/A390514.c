/*
 * A390514  Triangle read by rows: T(n,k) is the number of points missed by
 *          the longest non-intersecting loop starting at (0,0) on the n X k
 *          torus consisting of steps up and to the right, 1 <= k <= n.
 * https://oeis.org/A390514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, walk
 * Formula:   a(n) = T(r(k),c(n)) where T(N,K) = min{ (N*x) % K : 0 <= x < K ,
 *            gcd(K-x, floor(N*x/K)) = 1 } (OEIS A390514). The sequence is
 *            read by rows of the triangle
 * Range:     u8 n <= 810, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper header for gcd */
#ifndef _GCD_H_INCLUDED
#include "../utils/gcd.h"
#endif

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n */
    idx_t row = 1;
    while ((row * (row + 1)) / 2 < n)
        ++row;

    idx_t prev = (row - 1) * row / 2;          /* sum of previous rows */
    seq_t k   = (seq_t)(n - prev);             /* column index (1‑based) */
    seq_t N   = (seq_t)row;                    /* row size (torus dimension) */

    seq_t best = k;                            /* initialise with a value > possible mods */

    for (idx_t x = 0; x < (idx_t)k; ++x) {
        seq_t num = N * (seq_t)x;              /* N*x   */
        seq_t q   = num / k;                   /* floor(N*x / K) */
        seq_t rmod = num % k;                  /* (N*x) mod K   */
        seq_t kx = k - (seq_t)x;               /* K - x          */

        if (gcd(kx, q) == 1) {
            if (rmod < best)
                best = rmod;
        }
    }
    return best; /* T(N,K) */
}
