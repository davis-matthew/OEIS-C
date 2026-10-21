/*
 * A268786  Number of nX5 binary arrays with some element plus some
 *          horizontally, vertically or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A268786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   counts n×5 binary arrays with exactly one adjacent pair of ones
 *            * (adjacent horizontally, vertically, or anti‑diagonally). The
 *            DP below * iterates over rows, keeping the mask of the previous
 *            row and whether * exactly one adjacent pair has already occurred
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* 5‑bit masks, 0 … 31 */
#define M 32

seq_t a(idx_t n)
{
    /* horizontal edge counts for each mask */
    unsigned char h[M];
    for (unsigned m = 0; m < M; ++m) {
        unsigned cnt = 0;
        for (unsigned j = 0; j < 4; ++j)
            if (((m >> j) & 1U) && ((m >> (j + 1)) & 1U))
                ++cnt;
        h[m] = (unsigned char)cnt;
    }

    /* cross‑row edge counts (vertical + anti‑diagonal) */
    unsigned char cross[M][M];
    for (unsigned pm = 0; pm < M; ++pm) {
        for (unsigned nm = 0; nm < M; ++nm) {
            unsigned cnt = 0;
            /* vertical edges */
            for (unsigned j = 0; j < 5; ++j)
                if (((pm >> j) & 1U) && ((nm >> j) & 1U))
                    ++cnt;
            /* anti‑diagonal edges (down‑right) */
            for (unsigned j = 0; j < 4; ++j)
                if (((pm >> j) & 1U) && ((nm >> (j + 1)) & 1U))
                    ++cnt;
            cross[pm][nm] = (unsigned char)cnt;
        }
    }

    seq_t dp0[M] = {0};      /* ways with 0 adjacent pairs so far */
    seq_t dp1[M] = {0};      /* ways with exactly 1 adjacent pair so far */
    seq_t ndp0[M];
    seq_t ndp1[M];

    /* first row – only horizontal edges exist */
    for (unsigned m = 0; m < M; ++m) {
        if (h[m] == 0U)
            dp0[m] = (seq_t)1;
        else if (h[m] == 1U)
            dp1[m] = (seq_t)1;
        /* masks with >1 horizontal edge are ignored */
    }

    /* process remaining rows */
    for (idx_t row = 2; row <= n; ++row) {
        for (unsigned i = 0; i < M; ++i) {
            ndp0[i] = (seq_t)0;
            ndp1[i] = (seq_t)0;
        }

        for (unsigned pm = 0; pm < M; ++pm) {
            seq_t ways0 = dp0[pm];
            seq_t ways1 = dp1[pm];
            if (!ways0 && !ways1)
                continue;

            for (unsigned nm = 0; nm < M; ++nm) {
                unsigned added = (unsigned)h[nm] + (unsigned)cross[pm][nm];

                if (added == 0U) {
                    ndp0[nm] += ways0;
                    ndp1[nm] += ways1;
                } else if (added == 1U) {
                    ndp1[nm] += ways0;
                    /* ways1 would create 2 pairs → discarded */
                }
            }
        }

        for (unsigned i = 0; i < M; ++i) {
            dp0[i] = ndp0[i];
            dp1[i] = ndp1[i];
        }
    }

    /* sum all states with exactly one matching pair */
    seq_t result = (seq_t)0;
    for (unsigned i = 0; i < M; ++i)
        result += dp1[i];

    return result;
}
