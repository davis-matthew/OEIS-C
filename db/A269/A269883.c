/*
 * A269883  Number of n X 3 0..5 arrays with some element plus some
 *          horizontally, diagonally, antidiagonally or vertically adjacent
 *          neighbor totaling five exactly once.
 * https://oeis.org/A269883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function implements the OEIS definition of sequence
 *            A269883: number of n × 3 arrays with entries from {0,…,5} that
 *            contain exactly one adjacent pair (horizontal, vertical,
 *            diagonal, or anti‑diagonal) whose sum equals five. The
 *            implementation uses a dynamic programming recurrence over rows,
 *            a...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* The helper tables are small enough that a single static allocation
   suffices. */
static unsigned char row_vals[216][3];
static unsigned char within[216];
static unsigned char between[216][216];
static int tables_ready = 0;

/* Initialise the helper tables.  This is done exactly once. */
static void init_tables(void)
{
    if (tables_ready)
        return;

    for (int idx = 0; idx < 216; ++idx) {
        int v0 = idx % 6;
        int v1 = (idx / 6) % 6;
        int v2 = idx / 36;
        row_vals[idx][0] = (unsigned char)v0;
        row_vals[idx][1] = (unsigned char)v1;
        row_vals[idx][2] = (unsigned char)v2;
        unsigned char w = 0;
        if (v0 + v1 == 5) ++w;
        if (v1 + v2 == 5) ++w;
        within[idx] = w;
    }

    for (int prev = 0; prev < 216; ++prev) {
        for (int cur = 0; cur < 216; ++cur) {
            unsigned char e = 0;
            for (int c = 0; c < 3; ++c) {
                /* vertical neighbors */
                if (row_vals[cur][c] + row_vals[prev][c] == 5)
                    ++e;
                /* diagonal / anti‑diagonal */
                if (c > 0 &&
                    row_vals[cur][c] + row_vals[prev][c - 1] == 5)
                    ++e;
                if (c < 2 &&
                    row_vals[cur][c] + row_vals[prev][c + 1] == 5)
                    ++e;
            }
            between[prev][cur] = e;
        }
    }

    tables_ready = 1;
}

/* DP memoisation storage */
static seq_t dp0[216], dp1[216];
static seq_t new0[216], new1[216];
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t max_n = 0;

/* Ensure that the value for a(n) is computed and stored in cache[n]. */
static void ensure_computed(idx_t n)
{
    if (n < 1)
        return;

    init_tables();

    /* Grow the cache if necessary.  The allocation uses an explicit cast
       to satisfy C++'s stricter type rules. */
    if (cache_size <= n) {
        seq_t *tmp = NULL;
#if defined(__cplusplus)
        tmp = reinterpret_cast<seq_t*>(realloc(cache, (n + 1) * sizeof(seq_t)));
#else
        tmp = realloc(cache, (n + 1) * sizeof(seq_t));
#endif
        if (!tmp) exit(1);
        cache = tmp;
        /* Initialise new entries to zero.  The first entry (index 0)
           is unused, but keeping it zero is harmless. */
        for (idx_t i = cache_size; i <= n; ++i)
            cache[i] = 0;
        cache_size = n + 1;
    }

    /* Initialise DP for n = 1 once. */
    if (max_n == 0) {
        memset(dp0, 0, sizeof(dp0));
        memset(dp1, 0, sizeof(dp1));
        for (int cfg = 0; cfg < 216; ++cfg) {
            unsigned char w = within[cfg];
            if (w == 0)
                dp0[cfg] = 1;
            else if (w == 1)
                dp1[cfg] = 1;
        }
        seq_t s = 0;
        for (int i = 0; i < 216; ++i)
            if (dp1[i])
                s += dp1[i];
        cache[1] = s;
        max_n = 1;
    }

    /* Extend the DP to rows max_n+1 … n. */
    for (idx_t row = max_n + 1; row <= n; ++row) {
        memset(new0, 0, sizeof(new0));
        memset(new1, 0, sizeof(new1));
        for (int prev = 0; prev < 216; ++prev) {
            seq_t prev0 = dp0[prev];
            seq_t prev1 = dp1[prev];
            if (prev0 == 0 && prev1 == 0)
                continue;
            for (int cur = 0; cur < 216; ++cur) {
                unsigned char add = within[cur] + between[prev][cur];
                if (add == 0) {
                    if (prev0)
                        new0[cur] += prev0;
                    if (prev1)
                        new1[cur] += prev1;
                } else if (add == 1) {
                    if (prev0)
                        new1[cur] += prev0;
                }
            }
        }
        memcpy(dp0, new0, sizeof(dp0));
        memcpy(dp1, new1, sizeof(dp1));
        seq_t s = 0;
        for (int i = 0; i < 216; ++i)
            if (dp1[i])
                s += dp1[i];
        cache[row] = s;
    }

    if (max_n < n)
        max_n = n;
}

/* Public entry point.  The function follows the expected signature. */
seq_t a(idx_t n)
{
    ensure_computed(n);
    return cache[n];
}
