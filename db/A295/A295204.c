/*
 * A295204  Number of nX7 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 2 or 4 1s.
 * https://oeis.org/A295204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A295204
 *  Number of n×7 binary arrays with each 1 horizontally or vertically
 *  adjacent to exactly 2 or 4 1s.  The implementation is a dynamic
 *  programming algorithm on rows.  Each row is represented as a 7‑bit
 *  pattern.  For a pair (top, cur) we pre–compute all possible
 *  bottom rows that make cur valid with respect to its two neighbours.
 *  Transitions are performed using 128‑bit masks split into two 64‑bit
 *  words; only set bits are iterated.  The algorithm runs in O(n)
 *  time with a small constant factor and uses only a few megabytes.
 *
 *  The function a(idx_t n) implements the OEIS recurrence for all
 *  n ≥ 1.  No assumptions are made about the order of calls; it uses
 *  static memoisation structures that are lazily initialised.
 *
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Returns true if every set bit in cur has 2 or 4 neighbours
 * (horizontally within cur, vertically within top/bot). */
static bool valid_row(uint8_t top, uint8_t cur, uint8_t bot)
{
    for (int c = 0; c < 7; ++c) {
        if (!(cur & (1U << c))) continue;
        int deg = 0;
        /* horizontal neighbours */
        if (c > 0 && (cur & (1U << (c - 1)))) ++deg;
        if (c < 6 && (cur & (1U << (c + 1)))) ++deg;
        /* vertical neighbours */
        if (top & (1U << c)) ++deg;
        if (bot & (1U << c)) ++deg;
        if (deg != 2 && deg != 4) return false;
    }
    return true;
}

/* Pre‑computed transition masks for each (top, cur) pair. */
static uint64_t *mask_low  = NULL;
static uint64_t *mask_high = NULL;
#define N_PAT 128
#define MASKS (N_PAT * N_PAT)

/* DP tables */
static seq_t dpPrev[N_PAT][N_PAT];
static seq_t dpNext[N_PAT][N_PAT];

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – behaviour below is irrelevant */

    /* One‑time initialisation of transition masks */
    static bool inited = false;
    if (!inited) {
        size_t total = MASKS;
        mask_low  = (uint64_t *)malloc(total * sizeof(uint64_t));
        mask_high = (uint64_t *)malloc(total * sizeof(uint64_t));
        for (size_t top = 0; top < N_PAT; ++top) {
            for (size_t cur = 0; cur < N_PAT; ++cur) {
                uint64_t low  = 0;
                uint64_t high = 0;
                for (size_t bot = 0; bot < N_PAT; ++bot) {
                    if (valid_row((uint8_t)top, (uint8_t)cur, (uint8_t)bot)) {
                        if (bot < 64)
                            low  |= 1ULL << bot;
                        else
                            high |= 1ULL << (bot - 64);
                    }
                }
                size_t idx = top * N_PAT + cur;
                mask_low[idx]  = low;
                mask_high[idx] = high;
            }
        }
        inited = true;
    }

    /* Initialise DP: top border is all zeros */
    for (int i = 0; i < N_PAT; ++i)
        for (int j = 0; j < N_PAT; ++j)
            dpPrev[i][j] = 0;
    for (int cur = 0; cur < N_PAT; ++cur)
        dpPrev[0][cur] = 1;        /* count every first row; validity will be checked later */

    /* Process rows 2 … n */
    for (idx_t step = 1; step < n; ++step) {
        /* Zero next table */
        for (int i = 0; i < N_PAT; ++i)
            for (int j = 0; j < N_PAT; ++j)
                dpNext[i][j] = 0;

        /* Transitions */
        for (int top = 0; top < N_PAT; ++top) {
            for (int cur = 0; cur < N_PAT; ++cur) {
                seq_t val = dpPrev[top][cur];
                if (!val) continue;
                size_t idx = top * N_PAT + cur;
                uint64_t low  = mask_low[idx];
                uint64_t high = mask_high[idx];

                /* iterate over set bits in low part */
                while (low) {
                    int r = __builtin_ctzll(low);
                    low &= low - 1;
                    dpNext[cur][r] += val;
                }
                /* iterate over set bits in high part */
                while (high) {
                    int r = 64 + __builtin_ctzll(high);
                    high &= high - 1;
                    dpNext[cur][r] += val;
                }
            }
        }

        /* swap dpPrev and dpNext for next iteration */
        for (int i = 0; i < N_PAT; ++i)
            for (int j = 0; j < N_PAT; ++j)
                dpPrev[i][j] = dpNext[i][j];
    }

    /* Final row must have all zero below: check validity with bot = 0 */
    seq_t result = 0;
    for (int top = 0; top < N_PAT; ++top) {
        for (int cur = 0; cur < N_PAT; ++cur) {
            seq_t val = dpPrev[top][cur];
            if (!val) continue;
            if (valid_row((uint8_t)top, (uint8_t)cur, 0))
                result += val;
        }
    }
    return result;
}
