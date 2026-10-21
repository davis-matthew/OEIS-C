/*
 * A282883  Number of nX6 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, vertical and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A282883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of n×6 0..1 arrays in which at most one element
 *            violates the rule that a 1 has at most one 1 among its
 *            horizontal, vertical and antidiagonal neighbors, where the
 *            antidiagonal neighbors are the two directions up‑right and
 *            down‑left. The sequence is indexed with a(1) being the first
 *            t...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Constants for the 6‑column width */
#define WIDTH   6
#define NUM_MASK (1 << WIDTH)
#define STATE_COUNT (NUM_MASK * NUM_MASK)

/* Precomputed table: tri[a][b][c] = 0,1 or 2 (more than one violation)
   when moving from state (a,b) to the next row c. */
static uint8_t tri[NUM_MASK][NUM_MASK][NUM_MASK];
static bool tables_built = false;

static void build_tables(void)
{
    if (tables_built) return;
    for (int a = 0; a < NUM_MASK; ++a) {
        for (int b = 0; b < NUM_MASK; ++b) {
            for (int c = 0; c < NUM_MASK; ++c) {
                uint8_t viol = 0;
                for (int d = 0; d < WIDTH; ++d) {
                    if (!(b & (1 << d))) continue;          /* no 1 at this cell */
                    int cnt = 0;
                    /* horizontal neighbors */
                    if (d > 0          && (b & (1 << (d - 1)))) cnt++;
                    if (d < WIDTH-1    && (b & (1 << (d + 1)))) cnt++;
                    /* vertical neighbors (up/down) */
                    if (a & (1 << d))                 cnt++;
                    if (c & (1 << d))                 cnt++;
                    /* antidiagonal neighbors (up‑right and down‑left) */
                    if (d < WIDTH-1 && (a & (1 << (d + 1)))) cnt++;
                    if (d > 0          && (c & (1 << (d - 1)))) cnt++;
                    if (cnt > 1) {
                        viol++;
                        if (viol > 1) break;               /* stop counting */
                    }
                }
                tri[a][b][c] = viol;
            }
        }
    }
    tables_built = true;
}

/* DP state arrays: dp0 – no violation yet, dp1 – one violation already */
/* All states are encoded as (prev_row << WIDTH) | cur_row (both 6‑bit masks) */
static seq_t dp0[STATE_COUNT];        /* no violation so far */
static seq_t dp1[STATE_COUNT];        /* exactly one violation so far */
static seq_t dp0_next[STATE_COUNT];
static seq_t dp1_next[STATE_COUNT];
static idx_t current_rows = 0;        /* number of rows processed so far */

/* Memoization cache for a(n) */
static seq_t *cache = NULL;
static idx_t cache_cap   = 0;
static idx_t cache_size  = 0;          /* indices up to cache_size-1 are stored */

static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) return;                /* if realloc fails we keep old cache */
    if (!cache) cache = new_ptr;
    else cache = new_ptr;
    for (idx_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1, but guard for safety */

    /* Return cached value if already computed */
    if (n < cache_size) return cache[n];

    build_tables();

    /* Initialise DP for the first row if this is the first call */
    if (current_rows == 0) {
        memset(dp0, 0, sizeof(dp0));
        memset(dp1, 0, sizeof(dp1));
        for (int m = 0; m < NUM_MASK; ++m) dp0[m] = 1;   /* state (0, m) */
        current_rows = 1;
    }

    /* Extend DP up to the requested n */
    while (current_rows < n) {
        memset(dp0_next, 0, sizeof(dp0_next));
        memset(dp1_next, 0, sizeof(dp1_next));
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t v0 = dp0[s];
            seq_t v1 = dp1[s];
            if (!v0 && !v1) continue;
            int a_row = s >> WIDTH;
            int b_row = s & (NUM_MASK - 1);
            for (int c_row = 0; c_row < NUM_MASK; ++c_row) {
                uint8_t viol = tri[a_row][b_row][c_row];
                int ns = (b_row << WIDTH) | c_row;
                if (viol == 0) {
                    if (v0) dp0_next[ns] += v0;
                    if (v1) dp1_next[ns] += v1;
                } else if (viol == 1) {
                    if (v0) dp1_next[ns] += v0;
                } /* violations >1 are discarded */
            }
        }
        memcpy(dp0, dp0_next, sizeof(dp0));
        memcpy(dp1, dp1_next, sizeof(dp1));
        ++current_rows;
    }

    /* Finalise by checking the last row against a dummy below row of all 0's */
    seq_t ans = 0;
    for (int s = 0; s < STATE_COUNT; ++s) {
        seq_t v0 = dp0[s];
        seq_t v1 = dp1[s];
        if (!v0 && !v1) continue;
        int a_row = s >> WIDTH;
        int b_row = s & (NUM_MASK - 1);
        uint8_t viol = tri[a_row][b_row][0];   /* below row is all 0's */
        if (v0 && viol == 1) ans += v0;
        if (v1 && viol == 0) ans += v1;
    }

    /* Store in cache */
    ensure_cache(n);
    cache[n] = ans;
    if (n + 1 > cache_size) cache_size = n + 1;   /* up to n is cached */

    return ans;
}
