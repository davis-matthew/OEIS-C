/*
 * A252064  Number of (n+2)X(7+2) 0..2 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 1 2 or 4.
 * https://oeis.org/A252064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by dynamic programming over admissible 9‑digit rows.
   a(n) counts (n+2)×9 0..2 arrays whose every 3×3 block has all
   row, column, main‑diagonal and anti‑diagonal sums ∉ {1,2,4}.                     */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool initialized = false;
static int   K;                               /* number of admissible rows   */
static uint8_t *row_digits = NULL;            /* K * 9 array of digits      */
static int   *pair_start = NULL;              /* prefix sums of transitions */
static int   *pair_trans_count = NULL;        /* number of transitions per pair */
static int   *trans_flat = NULL;              /* concatenated transition lists */
static seq_t *dp = NULL;                      /* current DP counts           */
static seq_t *dp_next = NULL;                 /* temporary DP counts         */
static seq_t *cache = NULL;                   /* cached a(n) values          */
static idx_t  cache_size = 0;                 /* capacity of cache array     */
static idx_t  max_n_done = 0;                 /* largest n already computed  */
static idx_t  cur_height = 2;                 /* current array height (rows) */

/* ------------------------------------------------------------------ */
/* 13‑window sum test for a tripl e of rows i,j,k                     */
static inline bool triple_ok(int i, int j, int k)
{
    const uint8_t *ri = row_digits + i * 9;
    const uint8_t *rj = row_digits + j * 9;
    const uint8_t *rk = row_digits + k * 9;

    for (int c = 0; c < 7; ++c) {
        /* column sums */
        int sc0 = ri[c] + rj[c] + rk[c];
        if (sc0 == 1 || sc0 == 2 || sc0 == 4) return false;
        int sc1 = ri[c + 1] + rj[c + 1] + rk[c + 1];
        if (sc1 == 1 || sc1 == 2 || sc1 == 4) return false;
        int sc2 = ri[c + 2] + rj[c + 2] + rk[c + 2];
        if (sc2 == 1 || sc2 == 2 || sc2 == 4) return false;

        /* diagonals */
        int md = ri[c] + rj[c + 1] + rk[c + 2];
        if (md == 1 || md == 2 || md == 4) return false;
        int ad = ri[c + 2] + rj[c + 1] + rk[c];
        if (ad == 1 || ad == 2 || ad == 4) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Build all data structures once                                  */
static void init(void)
{
    if (initialized) return;

    const int MAX_STATE = 19683; /* 3^9 */
    int *temp_rows = (int *)malloc(MAX_STATE * sizeof(int));
    uint8_t *temp_bits = (uint8_t *)malloc(MAX_STATE * 9 * sizeof(uint8_t));

    /* generate all admissible rows */
    int k = 0;
    for (int s = 0; s < MAX_STATE; ++s) {
        int x = s;
        for (int p = 0; p < 9; ++p) {          /* decompose s into base‑3 digits */
            temp_bits[9 * s + p] = (uint8_t)(x % 3);
            x /= 3;
        }
        bool ok = true;
        for (int c = 0; c < 7 && ok; ++c) {
            int sum = temp_bits[9 * s + c] + temp_bits[9 * s + c + 1] + temp_bits[9 * s + c + 2];
            if (sum == 1 || sum == 2 || sum == 4) ok = false;
        }
        if (ok) temp_rows[k++] = s;
    }
    K = k;

    /* copy admissible rows into a compact array */
    row_digits = (uint8_t *)malloc(K * 9 * sizeof(uint8_t));
    for (int r = 0; r < K; ++r) {
        int s = temp_rows[r];
        memcpy(row_digits + r * 9, temp_bits + 9 * s, 9 * sizeof(uint8_t));
    }
    free(temp_rows);
    free(temp_bits);

    /* transition tables for (i,j) → list of k */
    int pair_cnt = K * K;
    pair_trans_count = (int *)malloc(pair_cnt * sizeof(int));
    pair_start = (int *)malloc((pair_cnt + 1) * sizeof(int));

    int total_trans = 0;
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) {
            int cnt = 0;
            for (int kk = 0; kk < K; ++kk) {
                if (triple_ok(i, j, kk)) ++cnt;
            }
            pair_trans_count[i * K + j] = cnt;
            total_trans += cnt;
        }
    }
    pair_start[0] = 0;
    for (int idx = 0; idx < pair_cnt; ++idx) pair_start[idx + 1] = pair_start[idx] + pair_trans_count[idx];

    trans_flat = (int *)malloc(total_trans * sizeof(int));
    int pos = 0;
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) {
            for (int kk = 0; kk < K; ++kk) {
                if (triple_ok(i, j, kk)) trans_flat[pos++] = kk;
            }
        }
    }

    /* DP arrays */
    dp = (seq_t *)malloc(pair_cnt * sizeof(seq_t));
    dp_next = (seq_t *)malloc(pair_cnt * sizeof(seq_t));
    memset(dp, 0, pair_cnt * sizeof(seq_t));
    memset(dp_next, 0, pair_cnt * sizeof(seq_t));
    for (int idx = 0; idx < pair_cnt; ++idx) dp[idx] = 1; /* height = 2 */

    initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Return a(n)                                                        */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* offset 1 */

    init();                /* ensure data structures */

    /* enlarge cache if necessary */
    if (n + 1 > cache_size) {
        idx_t new_cap = cache_size < 32 ? 32 : cache_size * 2;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        for (idx_t i = cache_size; i < new_cap; ++i) cache[i] = 0;
        cache_size = new_cap;
    }

    /* compute up to desired n */
    while (max_n_done < n) {
        /* zero dp_next */
        memset(dp_next, 0, K * K * sizeof(seq_t));

        /* single transition step */
        for (int idx = 0; idx < K * K; ++idx) {
            seq_t val = dp[idx];
            if (val == 0) continue;
            int j = idx % K;
            int start = pair_start[idx];
            int cnt = pair_trans_count[idx];
            for (int t = 0; t < cnt; ++t) {
                int k = trans_flat[start + t];
                dp_next[j * K + k] += val;
            }
        }

        /* swap DP arrays */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;

        /* advance height */
        ++cur_height;

        /* total number of arrays of current height */
        seq_t total = 0;
        for (int idx = 0; idx < K * K; ++idx) total += dp[idx];

        /* store for n = cur_height - 2 */
        cache[cur_height - 2] = total;
        max_n_done = cur_height - 2;
    }

    return cache[n];
}
