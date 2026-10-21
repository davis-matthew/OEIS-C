/*
 * A251217  Number of (n+1)X(6+1) 0..1 arrays with no 2X2 subblock having zero
 *          or two 1s.
 * https://oeis.org/A251217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×7 binary arrays with no 2×2 subblock
 *            having zero or two 1's (OEIS A251217)
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* adjacency data for the 128 possible 7‑bit rows */
static unsigned char next_rows[128][128];
static unsigned char next_deg[128];

/* DP buffers for the counts of rows at each sequence length */
static seq_t rows[128];
static seq_t tmp[128];

/* cache of a(n) values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* memoisation state */
static idx_t computed_n = 0;
static int initialized = 0;

/* build the adjacency list and initialise DP buffers */
static void init(void)
{
    if (initialized) return;
    initialized = 1;

    /* adjacency computation */
    for (idx_t i = 0; i < 128; ++i) {
        next_deg[i] = 0;
        for (idx_t j = 0; j < 128; ++j) {
            bool ok = true;
            for (idx_t col = 0; col < 6; ++col) {
                int sum =
                    ((i >> col) & 1) + ((i >> (col + 1)) & 1) +
                    ((j >> col) & 1) + ((j >> (col + 1)) & 1);
                if (sum == 0 || sum == 2) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                next_rows[i][next_deg[i]] = (unsigned char)j;
                next_deg[i] += 1;
            }
        }
    }

    /* initialise DP: one row allowed for each state */
    for (idx_t i = 0; i < 128; ++i) {
        rows[i] = 0;
    }
    for (idx_t i = 0; i < 128; ++i) {
        rows[i] = 1;
    }

    /* initialise cache */
    cache_cap = 4;
    cache = seq_alloc(cache_cap);
    cache_len  = 0;
    computed_n = 0;
}

seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n <= computed_n) return cache[n - 1];

    while (computed_n < n) {
        /* zero temporary buffer */
        for (idx_t i = 0; i < 128; ++i) tmp[i] = 0;

        /* transition */
        for (idx_t i = 0; i < 128; ++i) {
            if (rows[i] == 0) continue;
            for (unsigned char t = 0; t < next_deg[i]; ++t) {
                unsigned char j = next_rows[i][t];
                tmp[j] += rows[i];
            }
        }

        /* copy to main DP buffer and compute sum */
        seq_t total = 0;
        for (idx_t i = 0; i < 128; ++i) {
            rows[i] = tmp[i];
            total   += rows[i];
        }

        /* grow cache if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap << 1;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }

        cache[cache_len++] = total;
        computed_n += 1;
    }

    return cache[n - 1];
}
