/*
 * A296962  Number of nX7 0..1 arrays with each 1 adjacent to 2, 3 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of n×7 binary arrays where every 1 has 2, 3 or 5
 *            king‑move neighboring 1s. The code implements an exact DP
 *            algorithm with memoisation
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/* compile‑time constants */
enum { WIDTH = 7, PAT = 1 << WIDTH, STATE = PAT * PAT };

/* adjacency pre‑computation */
static uint8_t next_rows[STATE][PAT];
static uint8_t next_cnt[STATE];
static bool   bestzero[STATE];
static bool   adj_ready = false;

/* DP arrays and pointers for swapping */
static seq_t dp_arr[STATE];
static seq_t ndp_arr[STATE];
static seq_t *dp_ptr = dp_arr;
static seq_t *ndp_ptr = ndp_arr;

/* cache of already computed sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t  cache_max  = 0;

/* neighbour‑count check for a 1 in `cur` */
static inline bool validity(int up, int cur, int down) {
    for (int col = 0; col < WIDTH; ++col) {
        if ((cur >> col) & 1) {
            int cnt = 0;
            /* same row */
            if (col > 0      && (cur       >> (col-1)) & 1) ++cnt;
            if (col + 1 < WIDTH && (cur      >> (col+1)) & 1) ++cnt;
            /* upper row */
            if (col > 0      && (up        >> (col-1)) & 1) ++cnt;
            if (up          >> col & 1)                         ++cnt;
            if (col + 1 < WIDTH && (up        >> (col+1)) & 1) ++cnt;
            /* lower row */
            if (col > 0      && (down      >> (col-1)) & 1) ++cnt;
            if (down        >> col & 1)                         ++cnt;
            if (col + 1 < WIDTH && (down      >> (col+1)) & 1) ++cnt;
            if (!(cnt == 2 || cnt == 3 || cnt == 5)) return false;
        }
    }
    return true;
}

/* build adjacency tables and the `bestzero` array */
static void compute_adjacency(void) {
    for (int up = 0; up < PAT; ++up) {
        for (int cur = 0; cur < PAT; ++cur) {
            int idx = up * PAT + cur;
            uint8_t c = 0;
            for (int down = 0; down < PAT; ++down) {
                if (validity(up, cur, down)) {
                    next_rows[idx][c++] = (uint8_t)down;
                }
            }
            next_cnt[idx] = c;
            bestzero[idx] = validity(up, cur, 0);
        }
    }
    adj_ready = true;
}

/* make sure the cache has room for index n */
static void ensure_cache_size(idx_t n) {
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* initialise DP for the first row */
static void init_state(void) {
    const int zero = 0;
    for (int i = 0; i < STATE; ++i) dp_ptr[i] = 0;
    for (int cur = 0; cur < PAT; ++cur)
        dp_ptr[zero * PAT + cur] = 1;
}

/* return a(n) for the current DP state (i.e. when the last
   constructed row is stored in dp_ptr) */
static seq_t compute_answer_current(void) {
    seq_t ans = 0;
    for (int idx = 0; idx < STATE; ++idx)
        if (bestzero[idx] && dp_ptr[idx] != 0)
            ans += dp_ptr[idx];
    return ans;
}

/* public sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* undefined, but harmless */
    if (!adj_ready) compute_adjacency();      /* one‑time set‑up */
    ensure_cache_size(n);

    /* if we already have it, return immediately */
    if (n <= cache_max) return cache[n];

    /* first call: build the state for n == 1 */
    if (cache_max == 0) {
        init_state();
        cache[1] = compute_answer_current();
        cache_max = 1;
    }

    /* extend DP to the required length */
    for (idx_t row = cache_max + 1; row <= n; ++row) {
        /* clear next DP */
        for (int i = 0; i < STATE; ++i) ndp_ptr[i] = 0;

        /* transition from every (up,cur) state */
        for (int up = 0; up < PAT; ++up) {
            int up_off = up * PAT;
            for (int cur = 0; cur < PAT; ++cur) {
                int state_idx = up_off + cur;
                seq_t val = dp_ptr[state_idx];
                if (val == 0) continue;
                uint8_t *poss = next_rows[state_idx];
                uint8_t c = next_cnt[state_idx];
                for (uint8_t k = 0; k < c; ++k) {
                    int down = poss[k];
                    int next_idx = cur * PAT + down;
                    ndp_ptr[next_idx] += val;
                }
            }
        }

        /* swap current and next DP buffers */
        seq_t *tmp = dp_ptr;
        dp_ptr = ndp_ptr;
        ndp_ptr = tmp;

        /* store the answer for this row */
        cache[row] = compute_answer_current();
    }

    cache_max = n;
    return cache[n];
}
