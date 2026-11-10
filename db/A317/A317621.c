/*
 * A317621  Numbers missing from A317619.
 * https://oeis.org/A317621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th nonnegative integer not visited by the (2,4)-leaper
 *            walk of A317619 (search: simulate the walk until it is stuck,
 *            then list the unvisited labels)
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9465 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static inline long long A317621_absl(long long x) { return x < 0 ? -x : x; }

static uint64_t A317621_num(long long x, long long y)
{
    if (x == 0 && y == 0) return 0;
    long long r = A317621_absl(x) > A317621_absl(y) ? A317621_absl(x) : A317621_absl(y);
    uint64_t base = (uint64_t)(2 * r - 1) * (uint64_t)(2 * r - 1);
    long long off;
    if (x == r && y != -r) off = y + r - 1;
    else if (y == r && x != r) off = 2 * r + (r - 1 - x);
    else if (x == -r && y != r) off = 4 * r + (r - 1 - y);
    else off = 6 * r + (x + r - 1);
    return base + (uint64_t)off;
}

static bool A317621_ready = false;
static uint64_t *A317621_miss = NULL;
static uint64_t A317621_cnt = 0;
static uint64_t A317621_maxv = 0;

static void A317621_init(void)
{
    size_t cap = 1 << 16;
    uint8_t *vis = (uint8_t *)calloc(cap, 1);
    static const int mv[8][2] = {
        {2, 4}, {2, -4}, {-2, 4}, {-2, -4},
        {4, 2}, {4, -2}, {-4, 2}, {-4, -2}
    };
    long long x = 0, y = 0;
    vis[0] = 1;
    uint64_t maxv = 0;
    for (;;) {
        bool found = false;
        uint64_t best = 0;
        int bi = 0;
        for (int m = 0; m < 8; m++) {
            uint64_t t = A317621_num(x + mv[m][0], y + mv[m][1]);
            if (t >= cap) {
                size_t nc = cap;
                while (nc <= t) nc <<= 1;
                vis = (uint8_t *)realloc(vis, nc);
                memset(vis + cap, 0, nc - cap);
                cap = nc;
            }
            if (!vis[t] && (!found || t < best)) {
                found = true;
                best = t;
                bi = m;
            }
        }
        if (!found) break;
        x += mv[bi][0];
        y += mv[bi][1];
        vis[best] = 1;
        if (best > maxv) maxv = best;
    }
    uint64_t c = 0;
    for (uint64_t i = 0; i <= maxv; i++) if (!vis[i]) c++;
    A317621_miss = (uint64_t *)malloc((c + 1) * sizeof(uint64_t));
    uint64_t k = 0;
    for (uint64_t i = 0; i <= maxv; i++) if (!vis[i]) A317621_miss[k++] = i;
    A317621_cnt = c;
    A317621_maxv = maxv;
    free(vis);
    A317621_ready = true;
}

seq_t a(idx_t n)
{
    if (!A317621_ready) A317621_init();
    uint64_t m = (uint64_t)n;
    if (m <= A317621_cnt) return (seq_t)A317621_miss[m - 1];
    return (seq_t)A317621_maxv + (seq_t)(m - A317621_cnt);
}
