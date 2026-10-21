/*
 * A284071  Number of nX4 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly two elements.
 * https://oeis.org/A284071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts n×4 binary arrays with exactly two 1‑cells * having
 *            more than three horizontal, diagonal, or antidiagonal *
 *            neighbors. The value is computed by dynamic programming over *
 *            rows, tracking the last two rows and the number of violations so
 *            far. * See the explanation above for full details
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* --- precomputed row‑violation table ------------------------------------- */
static bool table_precomputed = false;
static int row_viol[16][16][16]; /* [row][up][down] */

static void precompute_row_viol(void) {
    if (table_precomputed) return;
    for (int row = 0; row < 16; ++row) {
        for (int up = 0; up < 16; ++up) {
            for (int down = 0; down < 16; ++down) {
                int viol = 0;
                for (int j = 0; j < 4; ++j) {
                    if (!(row & (1 << j))) continue;                      /* 1‑cell only */
                    int neigh = 0;
                    /* left/right */
                    if (j > 0 && (row & (1 << (j - 1)))) neigh++;
                    if (j < 3 && (row & (1 << (j + 1)))) neigh++;
                    /* up-left / up-right */
                    if (j > 0 && (up & (1 << (j - 1)))) neigh++;
                    if (j < 3 && (up & (1 << (j + 1)))) neigh++;
                    /* down-left / down-right */
                    if (j > 0 && (down & (1 << (j - 1)))) neigh++;
                    if (j < 3 && (down & (1 << (j + 1)))) neigh++;
                    if (neigh > 3) viol++;
                }
                row_viol[row][up][down] = viol;
            }
        }
    }
    table_precomputed = true;
}

/* --- DP state structures ----------------------------------------------- */
#define MAX_V 3               /* 0,1,2 violations kept */
#define ROWS 16

static inline void zero_dp(seq_t dp[MAX_V][ROWS][ROWS]) {
    for (int v = 0; v < MAX_V; ++v)
        for (int i = 0; i < ROWS; ++i)
            memset(dp[v][i], 0, ROWS * sizeof(seq_t));
}

/* ---- global cache for computed a(n) ------------------------------------ */
static seq_t *cache = NULL;
static size_t cache_alloc = 0;
static size_t cache_len   = 0;

/* ---- DP for current number of processed rows --------------------------- */
static size_t current_k = 0;                /* rows processed for which dp holds */
static seq_t dp_cur[MAX_V][ROWS][ROWS];
static seq_t dp_next[MAX_V][ROWS][ROWS];

/* ---- internal routine to extend DP to one more row ------------------- */
static void extend_one_row(void) {
    zero_dp(dp_next);
    for (int v = 0; v < MAX_V; ++v) {
        for (int prev = 0; prev < ROWS; ++prev) {
            for (int curr = 0; curr < ROWS; ++curr) {
                seq_t ways = dp_cur[v][prev][curr];
                if (!ways) continue;
                for (int nxt = 0; nxt < ROWS; ++nxt) {
                    int viol = row_viol[curr][prev][nxt];
                    int nv = v + viol;
                    if (nv >= MAX_V) continue;
                    dp_next[nv][curr][nxt] = (seq_t)(dp_next[nv][curr][nxt] + ways);
                }
            }
        }
    }
    /* swap dp_next into dp_cur */
    memcpy(dp_cur, dp_next, sizeof(dp_cur));
    ++current_k;
}

/* ---- internal routine to compute answer for current_k ----------------- */
static seq_t compute_answer_current(void) {
    seq_t ans = 0;
    for (int v = 0; v < MAX_V; ++v) {
        for (int prev = 0; prev < ROWS; ++prev) {
            for (int curr = 0; curr < ROWS; ++curr) {
                seq_t ways = dp_cur[v][prev][curr];
                if (!ways) continue;
                int v_final = v + row_viol[curr][prev][0]; /* down row = 0 */
                if (v_final == 2) {
                    ans = (seq_t)(ans + ways);
                }
            }
        }
    }
    return ans;
}

/* ---- public sequence function ------------------------------------------ */
static void ensure_cache_size(size_t n) {
    precompute_row_viol();
    while (cache_len < n) {
        if (cache_len == 0) {                /* a(1) */
            seq_t a1 = 0;
            for (int r = 0; r < ROWS; ++r) {
                if (row_viol[r][0][0] == 2) a1 = (seq_t)(a1 + (seq_t)1);
            }
            if (cache_len == cache_alloc) {
                size_t new_alloc = cache_alloc ? cache_alloc * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
                cache = new_cache;
                cache_alloc = new_alloc;
            }
            cache[cache_len++] = a1;
        } else if (cache_len == 1) {          /* compute a(2) and init dp_cur */
            zero_dp(dp_cur);
            for (int prev = 0; prev < ROWS; ++prev) {
                for (int curr = 0; curr < ROWS; ++curr) {
                    int viol = row_viol[prev][0][curr];
                    if (viol < MAX_V)
                        dp_cur[viol][prev][curr] = (seq_t)1;
                }
            }
            current_k = 2;
            seq_t a2 = 0;
            a2 = compute_answer_current();
            if (cache_len == cache_alloc) {
                size_t new_alloc = cache_alloc ? cache_alloc * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
                cache = new_cache;
                cache_alloc = new_alloc;
            }
            cache[cache_len++] = a2;
        } else {                              /* extend to next row */
            extend_one_row();
            seq_t an = compute_answer_current();
            if (cache_len == cache_alloc) {
                size_t new_alloc = cache_alloc ? cache_alloc * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
                cache = new_cache;
                cache_alloc = new_alloc;
            }
            cache[cache_len++] = an;
        }
    }
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;    /* offset starts at 1, but just in case */
    ensure_cache_size((size_t)n);
    return cache[n - 1];
}
