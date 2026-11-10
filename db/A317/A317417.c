/*
 * A317417  Numbers missing from A317415.
 * https://oeis.org/A317417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th nonnegative integer not visited by the (1,4)-leaper
 *            walk of A317415 (simulate the walk until stuck, then list the
 *            unvisited numbers)
 * Range:     u8 none, u16 n <= 52433, u32 n <= 95358, u64 n <= 95358,
 *            u128 n <= 95358, big any n
 * Validated: standard, pass, 49 terms through n = 9770 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static const int A317417_dx[8] = {1, 1, -1, -1, 4, 4, -4, -4};
static const int A317417_dy[8] = {4, -4, 4, -4, 1, -1, 1, -1};

static uint64_t A317417_c2n(int64_t x, int64_t y)
{
    if (x == 0 && y == 0) return 0;
    int64_t ax = x >= 0 ? x : -x;
    int64_t ay = y >= 0 ? y : -y;
    int64_t k = ax > ay ? ax : ay;
    uint64_t nmin = (uint64_t)((2 * (k - 1) + 1) * (2 * (k - 1) + 1));
    int64_t off;
    if (x == k && y >= -(k - 1) && y <= k) off = y + (k - 1);
    else if (y == k && x <= k - 1 && x >= -k) off = 2 * k + ((k - 1) - x);
    else if (x == -k && y <= k - 1 && y >= -k) off = 4 * k + ((k - 1) - y);
    else off = 6 * k + (x + k - 1);
    return nmin + (uint64_t)off;
}

static uint8_t *A317417_vis = NULL;
static size_t A317417_cap = 0;

static void A317417_ensure(uint64_t need)
{
    if (need < A317417_cap) return;
    size_t nc = A317417_cap ? A317417_cap : 1024;
    while (nc <= (size_t)need) nc <<= 1;
    uint8_t *p = (uint8_t *)realloc(A317417_vis, nc);
    if (!p) abort();
    memset(p + A317417_cap, 0, nc - A317417_cap);
    A317417_vis = p;
    A317417_cap = nc;
}

static uint64_t *A317417_miss = NULL;
static size_t A317417_m = 0;
static uint64_t A317417_maxv = 0;
static bool A317417_init = false;

static void A317417_setup(void)
{
    int64_t cx = 0, cy = 0;
    A317417_ensure(0);
    A317417_vis[0] = 1;
    A317417_maxv = 0;
    for (;;) {
        uint64_t best = (uint64_t)-1;
        int64_t bx = cx, by = cy;
        for (int d = 0; d < 8; ++d) {
            int64_t nx = cx + A317417_dx[d], ny = cy + A317417_dy[d];
            uint64_t num = A317417_c2n(nx, ny);
            A317417_ensure(num);
            if (!A317417_vis[num] && num < best) { best = num; bx = nx; by = ny; }
        }
        if (best == (uint64_t)-1) break;
        A317417_vis[best] = 1;
        if (best > A317417_maxv) A317417_maxv = best;
        cx = bx; cy = by;
    }
    A317417_miss = (uint64_t *)malloc((A317417_maxv + 1) * sizeof(uint64_t));
    if (!A317417_miss) abort();
    for (uint64_t i = 0; i <= A317417_maxv; ++i)
        if (!A317417_vis[i]) A317417_miss[A317417_m++] = i;
    A317417_init = true;
}

seq_t a(idx_t n)
{
    if (!A317417_init) A317417_setup();
    if (n >= 1 && n <= (idx_t)A317417_m) return (seq_t)A317417_miss[n - 1];
    return (seq_t)A317417_maxv + (seq_t)(n - (idx_t)A317417_m);
}
