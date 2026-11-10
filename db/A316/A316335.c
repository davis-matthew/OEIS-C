/*
 * A316335  Numbers missing from A316588.
 * https://oeis.org/A316335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer not in A316588 (finite greedy
 *            knight walk, found by simulation)
 * Range:     u8 none, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A316335_vis = NULL;
static size_t A316335_len = 0;
static unsigned char *A316335_mark = NULL;
static uint64_t A316335_max = 0;
static bool A316335_ready = false;

static uint64_t A316335_board(uint64_t x, uint64_t y)
{
    uint64_t d = x + y - 2;
    return d * (d + 1) / 2 + d + 2 - x;
}

static void A316335_init(void)
{
    const size_t CAP = 200000;
    static const int dx[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };
    static const int dy[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    A316335_vis = (uint64_t *)malloc(CAP * sizeof(uint64_t));
    if (!A316335_vis) abort();
    A316335_vis[0] = 1;
    A316335_len = 1;
    int64_t kx = 1, ky = 1;
    while (A316335_len < CAP) {
        bool found = false;
        uint64_t best = 0;
        int64_t bx = 0, by = 0;
        for (int i = 0; i < 8; ++i) {
            int64_t cx = kx + dx[i], cy = ky + dy[i];
            if (cx < 1 || cy < 1) continue;
            uint64_t cand = A316335_board((uint64_t)cx, (uint64_t)cy);
            bool seen = false;
            for (size_t j = 0; j < A316335_len; ++j)
                if (A316335_vis[j] == cand) { seen = true; break; }
            if (seen) continue;
            if (!found || cand < best) {
                found = true; best = cand; bx = cx; by = cy;
            }
        }
        if (!found) break;
        A316335_vis[A316335_len++] = best;
        kx = bx; ky = by;
    }
    A316335_max = 0;
    for (size_t j = 0; j < A316335_len; ++j)
        if (A316335_vis[j] > A316335_max) A316335_max = A316335_vis[j];
    A316335_mark = (unsigned char *)calloc((size_t)A316335_max + 2, 1);
    if (!A316335_mark) abort();
    for (size_t j = 0; j < A316335_len; ++j)
        A316335_mark[A316335_vis[j]] = 1;
    A316335_ready = true;
}

seq_t a(idx_t n)
{
    if (!A316335_ready) A316335_init();
    uint64_t cnt = 0;
    for (uint64_t k = 1; k <= A316335_max; ++k) {
        if (!A316335_mark[k]) {
            ++cnt;
            if (cnt == (uint64_t)n) return (seq_t)k;
        }
    }
    return (seq_t)(A316335_max + ((uint64_t)n - cnt));
}
