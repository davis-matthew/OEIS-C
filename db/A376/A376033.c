/*
 * A376033  Number A(n,k) of binary words of length n avoiding distance (i+1)
 *          between "1" digits if the i-th bit is set in the binary
 *          representation of k; square array A(n,k), n>=0, k>=0, read by
 *          antidiagonals.
 * https://oeis.org/A376033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   a(n) of OEIS A376033 computed by dynamic programming. * For each
 *            index n, the pair (i,j) is obtained from antidiagonal_coords().
 *            * A(i,j) counts binary words of length i that avoid a distance d
 *            between * two '1's whenever the (d‑1)-st bit of j is set. The DP
 *            keeps a mask * of the presence of '1's...
 * Range:     u8 n <= 43, u16 n <= 151, u32 n <= 559, u64 n <= 2143,
 *            u128 n <= 8383, big any n
 * Helpers:   antidiagonal_coords, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/antidiagonal_coords.h"
#include "../utils/seq_pow.h"

/* Compute A(n,k) */
static seq_t compute_A(idx_t n, idx_t k)
{
    if (n == 0) return (seq_t)1;

    /* Determine maximal distance that can appear in D (bits of k) */
    idx_t tmp = k;
    idx_t D_max = 0;
    while (tmp) { ++D_max; tmp >>= 1; }

    /* No forbidden distances => 2^n */
    if (D_max == 0) return seq_pow((seq_t)2, n);

    /* Bitmask of forbidden distances (bit d-1 means distance d is forbidden) */
    uint64_t forb_mask = 0;
    for (idx_t bit = 0; bit < D_max; ++bit)
        if ((k >> bit) & 1ULL) forb_mask |= ((uint64_t)1 << bit);

    uint64_t mask_limit = ((uint64_t)1 << D_max) - 1;

    /* DP arrays holding counts for each mask (size 256 is enough for D_max ≤ 8) */
    static seq_t dp[256];
    static seq_t ndp[256];

    for (int m = 0; m < 256; ++m) { dp[m] = 0; ndp[m] = 0; }
    dp[0] = (seq_t)1;

    for (idx_t pos = 0; pos < n; ++pos) {
        for (int m = 0; m < 256; ++m) ndp[m] = 0;
        for (uint64_t mask = 0; mask <= mask_limit; ++mask) {
            seq_t val = dp[mask];
            if (val == (seq_t)0) continue;

            /* Place a 0 at this position */
            uint64_t m0 = (mask << 1) & mask_limit;
            ndp[m0] += val;

            /* Place a 1 at this position if it does not violate any forbidden distance */
            if ((mask & forb_mask) == 0) {
                uint64_t m1 = ((mask << 1) | 1ULL) & mask_limit;
                ndp[m1] += val;
            }
        }
        for (int m = 0; m < 256; ++m) dp[m] = ndp[m];
    }

    seq_t result = 0;
    for (uint64_t mask = 0; mask <= mask_limit; ++mask) result += dp[mask];
    return result;
}

/* External linkage required for the wheel */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return compute_A(i, j);
}
