/*
 * A268883  Number of nX5 binary arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two
 *          exactly once.
 * https://oeis.org/A268883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computes the number of n×5 binary arrays that contain some
 *            element plus a horizontally or antidiagonally adjacent neighbor
 *            totalling two exactly once. The function implements a dynamic
 *            programming solution that counts such arrays for all n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
#include <new>
#endif

/* Pre‑compute horizontal and anti‑diagonal adjacency counts for 5‑bit
   rows. */
static unsigned char horiz[32];               /* horizontal adjacent 1‑pairs */
static unsigned char anti[32][32];            /* anti‑diagonal pairs between rows */
static bool precomputed = false;

static void precompute(void)
{
    if (precomputed) return;
    precomputed = true;
    for (int p = 0; p < 32; ++p) {
        unsigned char h = 0;
        for (int j = 0; j < 4; ++j)
            if (((p >> j) & 1U) && ((p >> (j + 1)) & 1U))
                ++h;
        horiz[p] = h;
        for (int q = 0; q < 32; ++q)
            anti[p][q] = __builtin_popcount(((p << 1) & q));
    }
}

/* DP state for two consecutive rows, capped at 2 adjacent pairs.
   dp[pattern][count] 0 <= pattern < 32, 0 <= count <= 2 */
static seq_t dp_prev[32][3];
static seq_t dp_curr[32][3];

/* Cache of sequence values */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t computed_max = 0;

/* Ensure cache contains values up to n */
static void ensure_cache(idx_t n)
{
    if (n <= computed_max) return;

    /* Allocate or grow cache array */
#ifdef __cplusplus
    /* In C++ we use new/delete to respect class constructors. */
    seq_t *tmp = new seq_t[n + 1];
    for (size_t i = 0; i < cache_size; ++i)
        tmp[i] = cache[i];
    delete[] cache;
    cache = tmp;
    cache_size = n + 1;
#else
    /* In C we can use realloc, but cast the void* appropriately. */
    cache = realloc(cache, (n + 1) * sizeof(seq_t));
    cache_size = n + 1;
#endif

    precompute();

    for (idx_t cur = computed_max + 1; cur <= n; ++cur) {
        if (cur == 1) { /* initialise for the first row */
            for (int p = 0; p < 32; ++p)
                for (int c = 0; c < 3; ++c)
                    dp_prev[p][c] = 0;
            for (int p = 0; p < 32; ++p) {
                int c = horiz[p];
                if (c > 2) c = 2;
                dp_prev[p][c] = 1;
            }
            seq_t sum = 0;
            for (int p = 0; p < 32; ++p)
                sum += dp_prev[p][1];
            cache[1] = sum;
        } else { /* extend DP by one more row */
            for (int p = 0; p < 32; ++p)
                for (int c = 0; c < 3; ++c)
                    dp_curr[p][c] = 0;
            for (int pp = 0; pp < 32; ++pp) {
                for (int old = 0; old < 3; ++old) {
                    seq_t val = dp_prev[pp][old];
                    if (!val) continue;
                    for (int cp = 0; cp < 32; ++cp) {
                        int newpairs = horiz[cp] + anti[pp][cp];
                        int newcnt = old + newpairs;
                        if (newcnt > 2) newcnt = 2;
                        dp_curr[cp][newcnt] += val;
                    }
                }
            }
            /* copy dp_curr to dp_prev for next iteration */
            for (int p = 0; p < 32; ++p)
                for (int c = 0; c < 3; ++c)
                    dp_prev[p][c] = dp_curr[p][c];
            seq_t sum = 0;
            for (int p = 0; p < 32; ++p)
                sum += dp_prev[p][1];
            cache[cur] = sum;
        }
    }
    computed_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_cache(n);
    return cache[n];
}
