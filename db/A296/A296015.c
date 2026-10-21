/*
 * A296015  Number of n X 4 0..1 arrays with each 1 adjacent to 1 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Implements A296015 via a 3‑row transfer‑matrix DP. Count n×4
 *            binary arrays where each 1 has exactly 1 or 4 king‑move
 *            neighboring 1s. Uses a lazy pre‑computation of all 16³ possible
 *            triples of rows
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   valid_triple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/valid_triple.h"

static bool precomp[16][16][16];
static bool precomp_done = false;

/* Lazy pre‑computation of valid triples */
static void init_precomp(void)
{
    if (precomp_done) return;
    for (unsigned int up = 0; up < 16; ++up)
        for (unsigned int mid = 0; mid < 16; ++mid)
            for (unsigned int down = 0; down < 16; ++down)
                precomp[up][mid][down] = valid_triple((unsigned char)up,
                                                       (unsigned char)mid,
                                                       (unsigned char)down);
    precomp_done = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    init_precomp();

    /* DP state: pair (prevPrev, prev) */
    const size_t SZ = 256;                 /* 16 × 16 */
    seq_t dp[256] = {0}, nextdp[256] = {0};

    dp[0] = (seq_t)1;                      /* state (0,0) */

    for (idx_t step = 1; step <= n; ++step) {
        for (size_t i = 0; i < SZ; ++i) nextdp[i] = 0;
        for (unsigned int up = 0; up < 16; ++up) {
            for (unsigned int mid = 0; mid < 16; ++mid) {
                seq_t cur = dp[up * 16 + mid];
                if (!cur) continue;
                for (unsigned int down = 0; down < 16; ++down) {
                    if (!precomp[up][mid][down]) continue;
                    nextdp[mid * 16 + down] += cur;
                }
            }
        }
        for (size_t i = 0; i < SZ; ++i) dp[i] = nextdp[i];
    }

    /* final validation for the last row using a trailing zero row */
    seq_t result = 0;
    for (unsigned int prev = 0; prev < 16; ++prev) {
        for (unsigned int last = 0; last < 16; ++last) {
            seq_t cur = dp[prev * 16 + last];
            if (!cur) continue;
            if (precomp[prev][last][0]) result += cur;
        }
    }
    return result;
}
