/*
 * A269080  Number of 6Xn binary arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A269080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n binary arrays that contain at most one *
 *            pair of adjacent 1s (adjacency being horizontal, diagonal or
 *            anti‑diagonal)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   count_adj
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_adj.h"

/* Helper data structures */
static uint8_t zero_next[64][64];
static uint8_t one_next[64][64];
static uint8_t zero_cnt[64];
static uint8_t one_cnt[64];

/* DP tables: prev_arr[i][u] = number of ways to build a column
 * ending with mask i and having u (0/1) adjacency pairs used so far
 */
static seq_t prev_arr[64][2];
static seq_t cur_arr [64][2];
static seq_t (*prev)[2] = prev_arr;
static seq_t (*cur )[2] = cur_arr;

/* Cache for sequence values.  The problem guarantees n up to about 10000.
   Size 10001 guarantees we can store all requested terms. */
#define MAX_N 10001
static seq_t cache[MAX_N];
static idx_t cache_len = 0;

/* Initialise adjacency lists and initial DP state */
static void init_structs(void)
{
    /* Build adjacency lists */
    for (int a = 0; a < 64; ++a) {
        zero_cnt[a] = 0;
        one_cnt[a]  = 0;
        for (int b = 0; b < 64; ++b) {
            int adj = count_adj(a, b);
            if (adj == 0) {
                zero_next[a][zero_cnt[a]++] = (uint8_t)b;
            } else if (adj == 1) {
                one_next[a][one_cnt[a]++]  = (uint8_t)b;
            }
        }
    }

    /* Initial column (length 1) */
    for (int i = 0; i < 64; ++i) {
        prev[i][0] = 1;  /* any mask is allowed, no adjacency yet */
        prev[i][1] = 0;
    }

    /* Store first term a(1) = 64 */
    seq_t ans = 0;
    for (int i = 0; i < 64; ++i) {
        ans += prev[i][0];
    }
    cache[0] = ans;
    cache_len = 1;
}

/* Extend the DP up to the required length n */
static void extend_to(idx_t n)
{
    while (cache_len < n) {
        /* clear current array */
        for (int i = 0; i < 64; ++i) {
            cur[i][0] = 0;
            cur[i][1] = 0;
        }

        /* transition from prev to cur */
        for (int m = 0; m < 64; ++m) {
            seq_t v0 = prev[m][0];
            seq_t v1 = prev[m][1];
            if (v0 != 0 || v1 != 0) {
                /* zero‑adjacency transitions */
                for (int k = 0; k < zero_cnt[m]; ++k) {
                    int nb = zero_next[m][k];
                    cur[nb][0] += v0;   /* used stays 0 */
                    cur[nb][1] += v1;   /* used stays 1 */
                }
                /* one‑adjacency transitions (only from used=0) */
                for (int k = 0; k < one_cnt[m]; ++k) {
                    int nb = one_next[m][k];
                    cur[nb][1] += v0;   /* used becomes 1 */
                }
            }
        }

        /* swap prev and cur */
        seq_t (*tmp)[2] = prev;
        prev = cur;
        cur = tmp;

        /* compute answer for this length */
        seq_t ans = 0;
        for (int i = 0; i < 64; ++i) {
            ans += prev[i][0];
            ans += prev[i][1];
        }

        /* store result in cache */
        cache[cache_len] = ans;
        ++cache_len;
    }
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0, but safe */
    static int initialized = 0;
    if (!initialized) {
        init_structs();
        initialized = 1;
    }
    if (n > MAX_N-1) {  /* safety, though problem limits n */
        /* Out of range: compute on the fly (though not expected) */
        extend_to(n);
    } else {
        extend_to(n);
    }
    return cache[n-1];
}
