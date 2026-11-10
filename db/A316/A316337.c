/*
 * A316337  Numbers missing from A316667.
 * https://oeis.org/A316337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer not visited by the knight walk of
 *            A316667 (simulated until the knight is stuck; complement search)
 * Range:     u8 none, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define A316337_R 150
#define A316337_D (2 * A316337_R + 1)
#define A316337_V ((uint64_t)A316337_D * A316337_D + 2)

static int64_t A316337_num(int64_t x, int64_t y)
{
    int64_t ax = x < 0 ? -x : x;
    int64_t ay = y < 0 ? -y : y;
    int64_t r = ax > ay ? ax : ay;
    int64_t base = (2 * r + 1) * (2 * r + 1);
    if (y == -r) return base - (r - x);
    if (x == -r) return base - 2 * r - (y + r);
    if (y == r)  return base - 4 * r - (x + r);
    return base - 6 * r - (r - y);
}

static unsigned char A316337_grid[A316337_D][A316337_D];
static unsigned char A316337_vis[A316337_V];
static bool A316337_ready = false;

static void A316337_init(void)
{
    static const int dx[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };
    static const int dy[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    int64_t cx = 0, cy = 0;
    A316337_grid[cx + A316337_R][cy + A316337_R] = 1;
    A316337_vis[1] = 1;
    for (;;) {
        bool found = false;
        int64_t bv = 0, bx = 0, by = 0;
        for (int i = 0; i < 8; ++i) {
            int64_t nx = cx + dx[i], ny = cy + dy[i];
            if (nx < -A316337_R || nx > A316337_R || ny < -A316337_R || ny > A316337_R)
                continue;
            if (A316337_grid[nx + A316337_R][ny + A316337_R])
                continue;
            int64_t v = A316337_num(nx, ny);
            if (!found || v < bv) {
                found = true; bv = v; bx = nx; by = ny;
            }
        }
        if (!found) break;
        cx = bx; cy = by;
        A316337_grid[cx + A316337_R][cy + A316337_R] = 1;
        if ((uint64_t)bv < A316337_V) A316337_vis[bv] = 1;
    }
    A316337_ready = true;
}

seq_t a(idx_t n)
{
    if (!A316337_ready) A316337_init();
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        ++k;
        if (k >= A316337_V || !A316337_vis[k])
            ++count;
    }
    return (seq_t)k;
}
