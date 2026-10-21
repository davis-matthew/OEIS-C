/*
 * A269763  Number of n X 4 0..5 arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling five
 *          exactly once.
 * https://oeis.org/A269763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 798*a(n-1) - 180699*a(n-2) + 8707236*a(n-3) -
 *            167225067*a(n-4) + 1392360966*a(n-5) - 4194764289*a(n-6) for n >
 *            7
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the first values are computed by a DP over rows of width 4 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define ROWS 1296            /* 6^4 possible rows */

seq_t a(idx_t n)
{
    /* static data initialized only once */
    static bool init_done = false;
    static seq_t init_val[8];       /* init_val[1..7] */
    static seq_t *cache = NULL;     /* memoised values */
    static idx_t cache_size = 0;
    static idx_t last_computed = 0;

    /* ---------- build initial values (1..7) by DP ---------------- */
    if (!init_done) {
        /* digits of each row and horizontal pair count */
        static uint8_t row_digits[ROWS][4];
        static uint8_t horiz_cnt[ROWS];
        static uint8_t vert_cnt[ROWS * ROWS];   /* vertical pairs between two rows */

        /* Pre‑compute row digits and horizontal pair numbers */
        for (int v = 0; v < ROWS; ++v) {
            int q = v;
            for (int c = 0; c < 4; ++c) {
                row_digits[v][c] = q % 6;
                q /= 6;
            }
            int h = 0;
            for (int c = 0; c < 3; ++c)
                if (row_digits[v][c] + row_digits[v][c + 1] == 5) ++h;
            horiz_cnt[v] = (uint8_t)h;
        }

        /* Pre‑compute vertical pair counts between all row pairs */
        for (int v1 = 0; v1 < ROWS; ++v1)
            for (int v2 = 0; v2 < ROWS; ++v2) {
                int v = 0;
                for (int c = 0; c < 4; ++c)
                    if (row_digits[v1][c] + row_digits[v2][c] == 5) ++v;
                vert_cnt[v1 * ROWS + v2] = (uint8_t)v;
            }

        /* DP arrays use unsigned __int128 to avoid overflow for n ≤ 7 */
        typedef unsigned __int128 dp_t;
        static dp_t dp_prev[ROWS][3];
        static dp_t dp_next[ROWS][3];

        /* initialise for first row */
        for (int v = 0; v < ROWS; ++v) {
            dp_prev[v][0] = dp_prev[v][1] = dp_prev[v][2] = 0;
            int h = horiz_cnt[v];
            if (h == 0) dp_prev[v][0] = 1;
            else if (h == 1) dp_prev[v][1] = 1;
            else               dp_prev[v][2] = 1;
        }
        /* a(1) */
        dp_t sum = 0;
        for (int v = 0; v < ROWS; ++v) sum += dp_prev[v][1];
        init_val[1] = (seq_t)sum;

        /* process rows 2 … 7 */
        for (int r = 2; r <= 7; ++r) {
            /* clear dp_next */
            for (int v = 0; v < ROWS; ++v)
                dp_next[v][0] = dp_next[v][1] = dp_next[v][2] = 0;

            /* transition */
            for (int vp = 0; vp < ROWS; ++vp) {
                dp_t pv0 = dp_prev[vp][0];
                dp_t pv1 = dp_prev[vp][1];
                dp_t pv2 = dp_prev[vp][2];
                if (pv0 == 0 && pv1 == 0 && pv2 == 0) continue;

                for (int vn = 0; vn < ROWS; ++vn) {
                    uint8_t vcols = vert_cnt[vp * ROWS + vn];
                    uint8_t hnext = horiz_cnt[vn];

                    /* state 0 */
                    if (pv0) {
                        uint8_t tot = vcols + hnext;
                        if (tot == 0)          dp_next[vn][0] += pv0;
                        else if (tot == 1)     dp_next[vn][1] += pv0;
                        else                    dp_next[vn][2] += pv0;
                    }
                    /* state 1 */
                    if (pv1) {
                        uint8_t tot = 1 + vcols + hnext; /* one previous pair */
                        if (tot == 1)          dp_next[vn][1] += pv1;
                        else                    dp_next[vn][2] += pv1;
                    }
                    /* state 2 or more */
                    if (pv2) {
                        dp_next[vn][2] += pv2;   /* always stays ≥2 */
                    }
                }
            }

            /* sum up for a(r) */
            sum = 0;
            for (int v = 0; v < ROWS; ++v) sum += dp_next[v][1];
            init_val[r] = (seq_t)sum;

            /* swap dp_prev and dp_next for next iteration */
            for (int v = 0; v < ROWS; ++v)
                dp_prev[v][0] = dp_next[v][0],
                dp_prev[v][1] = dp_next[v][1],
                dp_prev[v][2] = dp_next[v][2];
        }

        /* initialise cache with the first 7 terms */
        cache_size = 8;   /* indices 0..7 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; ++i) cache[i] = 0;
        for (idx_t i = 1; i <= 7; ++i) cache[i] = init_val[i];

        last_computed = 7;
        init_done = true;
    }

    /* ---------- answer queries ---------- */
    /* n <= 7 */
    if (n <= last_computed) return cache[n];

    /* ensure cache big enough */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (size_t i = cache_size; i < new_size; ++i) cache[i] = 0;
        cache_size = new_size;
    }

    /* compute values up to n using the 6‑term recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = (seq_t)798      * cache[i-1]      ;
        val += (seq_t)8707236      * cache[i-3];
        val += (seq_t)1392360966   * cache[i-5];
        val -= (seq_t)180699       * cache[i-2];
        val -= (seq_t)167225067    * cache[i-4];
        val -= (seq_t)4194764289   * cache[i-6];
        cache[i] = val;
    }
    last_computed = n;
    return cache[n];
}
