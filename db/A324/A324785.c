/*
 * A324785  Values of A005132 at its low points.
 * https://oeis.org/A324785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: generate Recaman's sequence R = A005132 (R(0)=0; R(k) =
 *            R(k-1)-k if nonnegative and new, else R(k-1)+k) and list R(k) at
 *            the low points, i.e. k = 0 or R(k-1) > R(k) < R(k+1) < R(k+2),
 *            in order of k
 * Range:     u8 n <= 9, u16 n <= 95, u32 n <= 230, u64 n <= 230,
 *            u128 n <= 230, big any n
 * Validated: standard, pass, 73 terms through n = 225 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint8_t *lp_vis = NULL;
static uint64_t lp_vis_size = 0;
static bool lp_init = false;
static uint64_t lp_i = 0;          /* index of last generated term */
static uint64_t lp_w[4];           /* last up to 4 terms: lp_w[3] = R(lp_i) */
static uint64_t *lp_res = NULL;
static uint64_t lp_cnt = 0, lp_cap = 0;

static bool lp_visited(uint64_t v) {
    return v < lp_vis_size && lp_vis[v];
}

static void lp_mark(uint64_t v) {
    if (v >= lp_vis_size) {
        uint64_t ns = lp_vis_size ? lp_vis_size : 1024;
        while (ns <= v) ns *= 2;
        uint8_t *p = (uint8_t *)realloc(lp_vis, (size_t)ns);
        memset(p + lp_vis_size, 0, (size_t)(ns - lp_vis_size));
        lp_vis = p;
        lp_vis_size = ns;
    }
    lp_vis[v] = 1;
}

static void lp_push(uint64_t v) {
    if (lp_cnt == lp_cap) {
        lp_cap = lp_cap ? lp_cap * 2 : 64;
        lp_res = (uint64_t *)realloc(lp_res, (size_t)(lp_cap * sizeof(uint64_t)));
    }
    lp_res[lp_cnt++] = v;
}

seq_t a(idx_t n) {
    if (!lp_init) {
        lp_init = true;
        lp_mark(0);
        lp_i = 0;
        lp_w[0] = lp_w[1] = lp_w[2] = 0;
        lp_w[3] = 0;
        lp_push(0); /* R(0) = 0 is the first low point */
    }
    while (lp_cnt < (uint64_t)n) {
        uint64_t i = lp_i + 1;
        uint64_t prev = lp_w[3];
        uint64_t x;
        if (prev >= i && !lp_visited(prev - i)) x = prev - i;
        else x = prev + i;
        lp_mark(x);
        lp_w[0] = lp_w[1];
        lp_w[1] = lp_w[2];
        lp_w[2] = lp_w[3];
        lp_w[3] = x;
        lp_i = i;
        /* candidate k = i-2 needs R(k-1), so k >= 1, i.e. i >= 3 */
        if (i >= 3) {
            uint64_t km1 = lp_w[0], k = lp_w[1], k1 = lp_w[2], k2 = lp_w[3];
            if (km1 > k && k < k1 && k1 < k2) lp_push(k);
        }
    }
    return (seq_t)lp_res[n - 1];
}
