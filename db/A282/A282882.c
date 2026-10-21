/*
 * A282882  Number of nX5 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, vertical and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A282882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 0..1 arrays in which exactly one element is
 *            allowed to have more than one of its horizontal, vertical, or
 *            antidiagonal neighbors equal to 1, all other 1's must have at
 *            most one such neighbor
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#ifndef __cplusplus
#include <stdlib.h>
#include <string.h>
#endif

/* Precomputed table of violations for any triple of rows.
   inc_table[a][b][c] = number of cells in row b that have more than
   one 1 among its horizontal, vertical and antidiagonal neighbors,
   when the rows above and below are a and c, respectively.
   Values are 0,1,2 (2 indicates more than one violation). */
static unsigned char inc_table[32][32][32];

static void init_inc_table(void)
{
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            for (int c = 0; c < 32; ++c) {
                int inc = 0;
                for (int j = 0; j < 5; ++j) {
                    if (!((b >> j) & 1)) continue; /* cell is 0 -> no violation */
                    int cnt = 0;
                    /* left neighbor */
                    if (j > 0) cnt += (b >> (j - 1)) & 1;
                    /* right neighbor */
                    if (j < 4) cnt += (b >> (j + 1)) & 1;
                    /* up neighbor */
                    cnt += (a >> j) & 1;
                    /* down neighbor */
                    cnt += (c >> j) & 1;
                    /* up-right neighbor (a, j+1) */
                    if (j < 4) cnt += (a >> (j + 1)) & 1;
                    /* down-left neighbor (c, j-1) */
                    if (j > 0) cnt += (c >> (j - 1)) & 1;
                    if (cnt > 1) {
                        ++inc;
                        if (inc > 1) break; /* we only need to know >1 */
                    }
                }
                inc_table[a][b][c] = (unsigned char)inc;
            }
}

/* Helper to convert state to flat index.
   Indices: a,b in [0,31], v in [0,1] */
#ifdef __cplusplus
static inline size_t idx(int a, int b, int v)
{
    return (((size_t)a << 5) | (size_t)b) << 1 | (size_t)v;
}
#else
static inline size_t idx(int a, int b, int v)
{
    return ((((size_t)a << 5) | (size_t)b) << 1 | (size_t)v);
}
#endif

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    static int inc_table_inited = 0;
    if (!inc_table_inited) {
        init_inc_table();
        inc_table_inited = 1;
    }

#ifdef __cplusplus
    std::vector<seq_t> dp(2048, 0), ndp(2048, 0);
#else
    static seq_t *dp = NULL, *ndp = NULL;
    if (!dp) {
        dp = (seq_t*)malloc(2048 * sizeof(seq_t));
        ndp = (seq_t*)malloc(2048 * sizeof(seq_t));
        if (!dp || !ndp) return 0;  /* allocation failure: return 0 */
        memset(dp, 0, 2048 * sizeof(seq_t));
        memset(ndp, 0, 2048 * sizeof(seq_t));
    }
#endif

    /* Initialize after first row (no violations yet) */
#ifdef __cplusplus
    std::fill(dp.begin(), dp.end(), 0);
#else
    memset(dp, 0, 2048 * sizeof(seq_t));
#endif
    for (int b = 0; b < 32; ++b) {
        dp[idx(0, b, 0)] = (seq_t)1;
    }

    /* Add rows 2 .. n */
    for (idx_t step = 1; step < n; ++step) {
#ifdef __cplusplus
        std::fill(ndp.begin(), ndp.end(), 0);
#else
        memset(ndp, 0, 2048 * sizeof(seq_t));
#endif
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                for (int v = 0; v <= 1; ++v) {
                    seq_t val = dp[idx(a, b, v)];
                    if (!val) continue;
                    const unsigned char *inc_row = inc_table[a][b];
                    for (int c = 0; c < 32; ++c) {
                        unsigned char inc = inc_row[c];
                        if (v + inc > 1) continue;
                        ndp[idx(b, c, v + inc)] += val;
                    }
                }
            }
        }
        /* swap dp and ndp */
#ifdef __cplusplus
        dp.swap(ndp);
#else
        seq_t *tmp = dp; dp = ndp; ndp = tmp;
#endif
    }

    /* Evaluate final (dummy) row = 0 */
    seq_t result = (seq_t)0;
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            for (int v = 0; v <= 1; ++v) {
                seq_t base = dp[idx(a, b, v)];
                if (!base) continue;
                unsigned char inc = inc_table[a][b][0];
                if (v + inc == 1) result += base;
            }
        }
    }
    return result;
}
