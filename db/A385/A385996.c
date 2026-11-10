/*
 * A385996  a(n) is the least k such that some value appears n times among
 *          A002487(0), ..., A002487(k).
 * https://oeis.org/A385996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that some value occurs n times among
 *            A002487(0..k); computed by a sequential scan using A002487(m+2)
 *            = (2*floor(A002487(m)/A002487(m+1))+1)*A002487(m+1) -
 *            A002487(m), with A002487(0)=0, A002487(1)=1
 * Range:     u8 n <= 20, u16 n <= 242, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9663 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *g_cnt = 0;
static size_t g_cap = 0;
static uint64_t *g_res = 0;
static size_t g_rescap = 0;
static uint64_t g_maxc = 0;
static uint64_t g_k = 0;   /* next index to process */
static uint64_t g_x = 1, g_y = 1; /* s(g_k), s(g_k+1) */

static void A385996_init(void) {
    g_cap = 1024;
    g_cnt = (uint32_t *)calloc(g_cap, sizeof(uint32_t));
    g_rescap = 1024;
    g_res = (uint64_t *)calloc(g_rescap, sizeof(uint64_t));
    /* process index 0: value 0 */
    g_cnt[0] = 1;
    g_maxc = 1;
    g_res[1] = 0;
    g_k = 1;
    g_x = 1;
    g_y = 1;
}

seq_t a(idx_t n) {
    if (!g_cnt) A385996_init();
    while (g_maxc < (uint64_t)n) {
        uint64_t x = g_x, y = g_y;
        if (x >= g_cap) {
            size_t nc = g_cap;
            while (nc <= x) nc *= 2;
            uint32_t *p = (uint32_t *)realloc(g_cnt, nc * sizeof(uint32_t));
            memset(p + g_cap, 0, (nc - g_cap) * sizeof(uint32_t));
            g_cnt = p;
            g_cap = nc;
        }
        uint32_t c = ++g_cnt[x];
        if ((uint64_t)c > g_maxc) {
            g_maxc = c;
            if (g_maxc >= g_rescap) {
                size_t nr = g_rescap * 2;
                uint64_t *p = (uint64_t *)realloc(g_res, nr * sizeof(uint64_t));
                memset(p + g_rescap, 0, (nr - g_rescap) * sizeof(uint64_t));
                g_res = p;
                g_rescap = nr;
            }
            g_res[g_maxc] = g_k;
        }
        g_x = y;
        g_y = (2 * (x / y) + 1) * y - x;
        g_k++;
    }
    return (seq_t)g_res[n];
}
