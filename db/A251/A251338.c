/*
 * A251338  Number of (n+1) X (3+1) 0..3 arrays with every 2 X 2 subblock
 *          summing to a nonzero multiple of 3.
 * https://oeis.org/A251338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A251338: Number of (n+1)×4 0..3 arrays with every
   2×2 subblock summing to a non‑zero multiple of 3.  The function
   computes a(n) for n ≥ 1.  The value for n = 0 is defined as 256
   (the number of 1×4 arrays).  The computation is done by dynamic
   programming over the 256 possible rows.  No hard‑coded table is
   used; the result is exact for any integer type supplied as
   seq_t. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* -------- adjacency of rows ------------------------------------ */
static bool adj_built = false;
static uint8_t adj_cnt[256];
static uint8_t adj[256][256];

static void build_adjacency(void)
{
    if (adj_built) return;
    for (int r = 0; r < 256; ++r) {
        int rd[4];
        for (int j = 0; j < 4; ++j) rd[j] = (r >> (2*j)) & 3;
        adj_cnt[r] = 0;
        for (int s = 0; s < 256; ++s) {
            int sd[4];
            for (int j = 0; j < 4; ++j) sd[j] = (s >> (2*j)) & 3;
            bool ok = true;
            for (int j = 0; j < 3 && ok; ++j) {
                int sum = rd[j] + sd[j] + rd[j+1] + sd[j+1];
                if (sum == 0 || (sum % 3) != 0) ok = false;
            }
            if (ok) adj[r][adj_cnt[r]++] = (uint8_t)s;
        }
    }
    adj_built = true;
}

/* ----------- main routine -------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)256;     /* not part of the OEIS range */

    /* ensure adjacency tables are available */
    build_adjacency();

    /* ---- static cache ---------------------------------------- */
    static seq_t *cache        = NULL;
    static idx_t  cache_cap    = 0;
    static idx_t  cache_len    = 0;   /* largest n whose value is cached */

    if (n >= cache_cap) {
        idx_t new_cap = n + 1;                 /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;             /* allocation failed */
        if (tmp != cache) {
            memset(tmp + cache_cap, 0,
                   (new_cap - cache_cap) * sizeof(seq_t));
        } else {
            memset(tmp + cache_cap, 0,
                   (new_cap - cache_cap) * sizeof(seq_t));
        }
        cache = tmp;
        cache_cap = new_cap;
    }
    if (cache_len == 0) cache[0] = (seq_t)256;   /* only used for n=0 */

    if (n <= cache_len) return cache[n];

    /* ---- DP state -------------------------------------------- */
    static seq_t dp_prev[256];
    static seq_t dp_curr[256];
    static idx_t  dp_step = 0;     /* number of computed transitions */

    if (dp_step == 0 && cache_len == 0) {      /* first run */
        for (int r = 0; r < 256; ++r) dp_prev[r] = (seq_t)1;
    }

    for (idx_t step = dp_step + 1; step <= n; ++step) {
        /* clear next state */
        for (int r = 0; r < 256; ++r) dp_curr[r] = (seq_t)0;

        /* transition */
        for (int r = 0; r < 256; ++r) {
            seq_t val = dp_prev[r];
            if (val == (seq_t)0) continue;
            uint8_t cnt = adj_cnt[r];
            for (uint8_t k = 0; k < cnt; ++k) {
                uint8_t s = adj[r][k];
                dp_curr[s] += val;
            }
        }

        /* sum to obtain a(step) */
        seq_t sum = (seq_t)0;
        for (int s = 0; s < 256; ++s) sum += dp_curr[s];
        cache[step] = sum;

        /* prepare for next step */
        for (int r = 0; r < 256; ++r) dp_prev[r] = dp_curr[r];
    }

    dp_step = n;
    cache_len = n;

    return cache[n];
}
