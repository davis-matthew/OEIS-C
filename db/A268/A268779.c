/*
 * A268779  Number of nX6 binary arrays with some element plus some
 *          horizontally, vertically, diagonally or antidiagonally adjacent
 *          neighbor totalling two no more than once.
 * https://oeis.org/A268779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of n×6 binary arrays with at most one adjacent
   pair of 1’s (adjacent horizontally, vertically, or diagonally).  The
   values are produced by a transfer‑matrix dynamic programming that keeps
   track of whether an adjacency pair has already been used. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offsets start at 1 */

    /* Pre‑compute data that depends only on the 6‑bit patterns. */
    static unsigned char internal_pairs[64];
    static unsigned char cross_pairs[64][64];
    static bool tables_initialized = false;

    if (!tables_initialized) {
        for (int p = 0; p < 64; ++p) {
            internal_pairs[p] = (unsigned char)__builtin_popcount((unsigned int)(p & (p >> 1)));
            for (int q = 0; q < 64; ++q) {
                unsigned int v1 = p & q;
                unsigned int v2 = (p << 1) & q;
                unsigned int v3 = (p >> 1) & q;
                cross_pairs[p][q] = (unsigned char)(
                    __builtin_popcount(v1) +
                    __builtin_popcount(v2) +
                    __builtin_popcount(v3));
            }
        }
        tables_initialized = true;
    }

    /* DP arrays: dp[pattern][used] where used = 0 or 1 adjacency pair already used. */
    static seq_t dp[64][2];
    static seq_t newdp[64][2];
    memset(dp, 0, sizeof(dp));

    /* Initialize for the first row.  Patterns with more than one internal
       adjacency pair are invalid and therefore ignored. */
    for (int p = 0; p < 64; ++p) {
        unsigned char use = internal_pairs[p];
        if (use > 1) continue;                /* invalid pattern */
        dp[p][use] = (seq_t)1;
    }

    /* Process subsequent rows. */
    for (idx_t row = 2; row <= n; ++row) {
        memset(newdp, 0, sizeof(newdp));
        for (int p1 = 0; p1 < 64; ++p1) {
            for (int used = 0; used <= 1; ++used) {
                seq_t ways = dp[p1][used];
                if (ways == 0) continue;
                for (int p2 = 0; p2 < 64; ++p2) {
                    unsigned char add_internal = internal_pairs[p2];
                    if (add_internal > 1) continue;      /* pattern invalid by itself */
                    unsigned char add_cross = cross_pairs[p1][p2];
                    unsigned int total_used = used + add_internal + add_cross;
                    if (total_used > 1) continue;         /* would exceed allowed adjacency */
                    newdp[p2][total_used] += ways;
                }
            }
        }
        memcpy(dp, newdp, sizeof(dp));
    }

    /* Sum all valid final states. */
    seq_t result = (seq_t)0;
    for (int p = 0; p < 64; ++p) {
        result += dp[p][0];
        result += dp[p][1];
    }
    return result;
}
