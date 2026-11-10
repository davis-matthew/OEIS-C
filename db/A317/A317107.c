/*
 * A317107  Numbers missing from A317105.
 * https://oeis.org/A317107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th nonnegative integer not visited by the (1,3)-leaper
 *            walk of A317105 (simulate the walk until stuck, then list the
 *            unvisited numbers)
 * Range:     u8 n <= 128, u16 n <= 61813, u32 n <= 67078, u64 n <= 67078,
 *            u128 n <= 67078, big any n
 * Validated: standard, pass, 81 terms through n = 9570 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A317107_spiral(int64_t x, int64_t y)
{
    int64_t ax = x >= 0 ? x : -x;
    int64_t ay = y >= 0 ? y : -y;
    int64_t l = ax > ay ? ax : ay;
    if (l == 0) return 0;
    uint64_t base = (uint64_t)(2 * l - 1);
    base = base * base;
    int64_t offset;
    if (x == l && y >= -(l - 1) && y <= l) offset = y + (l - 1);
    else if (y == l && x >= -l && x <= l - 1) offset = 3 * l - 1 - x;
    else if (x == -l && y >= -l && y <= l - 1) offset = 4 * l + (l - 1 - y);
    else if (y == -l && x >= -l + 1 && x <= l) offset = 6 * l + (x + l - 1);
    else offset = 0;
    return base + (uint64_t)offset;
}

static unsigned char *A317107_vis = NULL;
static uint64_t A317107_size = 0;
static bool A317107_ready = false;

static bool A317107_test(uint64_t i)
{
    return i < A317107_size && A317107_vis[i];
}

static void A317107_set(uint64_t i)
{
    if (i >= A317107_size) {
        uint64_t ns = A317107_size ? A317107_size : 1024;
        while (ns <= i) ns *= 2;
        unsigned char *p = (unsigned char *)realloc(A317107_vis, (size_t)ns);
        if (!p) abort();
        for (uint64_t k = A317107_size; k < ns; ++k) p[k] = 0;
        A317107_vis = p;
        A317107_size = ns;
    }
    A317107_vis[i] = 1;
}

static void A317107_init(void)
{
    static const int dx[8] = {1, 1, -1, -1, 3, 3, -3, -3};
    static const int dy[8] = {3, -3, 3, -3, 1, -1, 1, -1};
    int64_t cx = 0, cy = 0;
    A317107_set(0);
    for (;;) {
        bool found = false;
        uint64_t best = 0;
        int64_t bx = 0, by = 0;
        for (int d = 0; d < 8; ++d) {
            int64_t nx = cx + dx[d], ny = cy + dy[d];
            uint64_t c = A317107_spiral(nx, ny);
            if (!A317107_test(c) && (!found || c < best)) {
                found = true;
                best = c;
                bx = nx;
                by = ny;
            }
        }
        if (!found) break;
        A317107_set(best);
        cx = bx;
        cy = by;
    }
    A317107_ready = true;
}

seq_t a(idx_t n)
{
    if (!A317107_ready) A317107_init();
    uint64_t cnt = 0;
    uint64_t v = 0;
    for (;; ++v) {
        if (!A317107_test(v)) {
            ++cnt;
            if (cnt == (uint64_t)n) return (seq_t)v;
        }
    }
}
