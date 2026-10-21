/*
 * A269093  Number of nX4 0..3 arrays with some element plus some horizontally
 *          or vertically adjacent neighbor totalling three exactly once.
 * https://oeis.org/A269093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: DP over rows; count arrays of size n×4 with elements 0..3
   such that exactly one adjacent pair (horizontal or vertical) sums to 3. */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#define MAX_N 10000

/* ------------------------------------------------------------ */
/* precomputed tables                                         */
/* ------------------------------------------------------------ */
static uint8_t digits[256][4];
static uint8_t h_count[256];
static uint8_t vert[256][256];
static int tables_ready = 0;

static void build_tables(void)
{
    for (uint32_t cfg = 0; cfg < 256; ++cfg) {
        uint8_t d0 = cfg & 3;
        uint8_t d1 = (cfg >> 2) & 3;
        uint8_t d2 = (cfg >> 4) & 3;
        uint8_t d3 = (cfg >> 6) & 3;
        digits[cfg][0] = d0;
        digits[cfg][1] = d1;
        digits[cfg][2] = d2;
        digits[cfg][3] = d3;
        /* horizontal sum-3 pairs */
        uint8_t h = 0;
        if (d0 + d1 == 3) ++h;
        if (d1 + d2 == 3) ++h;
        if (d2 + d3 == 3) ++h;
        h_count[cfg] = h;
    }
    for (uint32_t a = 0; a < 256; ++a) {
        for (uint32_t b = 0; b < 256; ++b) {
            uint8_t v = 0;
            for (int col = 0; col < 4; ++col) {
                if (digits[a][col] + digits[b][col] == 3) ++v;
            }
            vert[a][b] = v;
        }
    }
    tables_ready = 1;
}

/* ------------------------------------------------------------ */
/* DP state                                                   */
/* ------------------------------------------------------------ */
static seq_t dp_prev[256][2];
static seq_t dp_next[256][2];
static seq_t cache[MAX_N + 1];   /* cache[n] holds a(n) for 1<=n<=MAX_N */
static size_t cache_n = 0;       /* highest n for which cache is valid */

/* ------------------------------------------------------------ */
/* public function                                            */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!tables_ready) build_tables();

    if (n < 1) return (seq_t)0;          /* undefined for n<1, return 0 */

    if (n <= cache_n) return cache[n];   /* already computed */

    /* compute up to n */
    /* initialise for first row if not done yet */
    if (cache_n == 0) {
        memset(dp_prev, 0, sizeof(dp_prev));
        for (size_t cfg = 0; cfg < 256; ++cfg) {
            uint8_t h = h_count[cfg];
            if (h <= 1) dp_prev[cfg][h] += (seq_t)1;
        }
        /* answer for n=1 */
        seq_t sum = 0;
        for (size_t cfg = 0; cfg < 256; ++cfg) sum += dp_prev[cfg][1];
        cache[1] = sum;
        cache_n = 1;
    }

    for (idx_t row = cache_n + 1; row <= n; ++row) {
        memset(dp_next, 0, sizeof(dp_next));
        for (size_t prev = 0; prev < 256; ++prev) {
            seq_t v0 = dp_prev[prev][0];
            seq_t v1 = dp_prev[prev][1];
            if (v0 == (seq_t)0 && v1 == (seq_t)0) continue;
            for (size_t cur = 0; cur < 256; ++cur) {
                uint8_t h = h_count[cur];
                uint8_t vv = vert[prev][cur];
                if (v0 != (seq_t)0) {
                    int t0 = (int)h + (int)vv;
                    if (t0 <= 1) dp_next[cur][t0] += v0;
                }
                if (v1 != (seq_t)0) {
                    int t1 = 1 + (int)h + (int)vv;
                    if (t1 <= 1) dp_next[cur][t1] += v1;
                }
            }
        }
        memcpy(dp_prev, dp_next, sizeof(dp_prev));

        seq_t sum = 0;
        for (size_t cfg = 0; cfg < 256; ++cfg) sum += dp_prev[cfg][1];
        cache[row] = sum;
    }

    cache_n = (size_t)n;
    return cache[n];
}
