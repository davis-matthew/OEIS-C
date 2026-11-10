/*
 * A316967  Same as A316884, except numbering of the squares starts at 0
 *          rather than 1.
 * https://oeis.org/A316967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A316884(n+1) - 1; computed by simulating the
 *            diagonal-move walk on the spiral board, always stepping to the
 *            lowest unvisited diagonal neighbour
 * Range:     u8 n <= 63, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 78 terms through n = 6719 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A316967_num_from_coord(int64_t x, int64_t y) {
    int64_t ax = x >= 0 ? x : -x;
    int64_t ay = y >= 0 ? y : -y;
    int64_t r = ax > ay ? ax : ay;
    if (r == 0) return 1;
    uint64_t side = 2 * (uint64_t)r;
    uint64_t max_num = (2 * (uint64_t)r + 1);
    max_num = max_num * max_num;
    uint64_t t;
    if (y == -r && x > -r) {
        t = (uint64_t)(r - x);
    } else if (x == -r) {
        t = side + (uint64_t)(y + r);
    } else if (y == r && x < r) {
        t = 2 * side + (uint64_t)(x + r);
    } else {
        t = 3 * side + (uint64_t)(r - y);
    }
    return max_num - t;
}

static uint64_t A316967_isqrt(uint64_t n) {
    uint64_t lo = 0, hi = n < 4294967295ULL ? n : 4294967295ULL;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (mid * mid <= n) lo = mid; else hi = mid - 1;
    }
    return lo;
}

static void A316967_coord_from_num(uint64_t num, int64_t *x, int64_t *y) {
    if (num == 1) { *x = 0; *y = 0; return; }
    uint64_t m = A316967_isqrt(num);
    uint64_t r;
    if (m * m == num) r = (m & 1) ? (m - 1) / 2 : m / 2;
    else r = (m + 1) / 2;
    uint64_t side = 2 * r;
    uint64_t max_num = (2 * r + 1);
    max_num = max_num * max_num;
    uint64_t t = max_num - num;
    int64_t rr = (int64_t)r;
    if (t < side) {
        *x = rr - (int64_t)t; *y = -rr;
    } else if (t < 2 * side) {
        t -= side;
        *x = -rr; *y = -rr + (int64_t)t;
    } else if (t < 3 * side) {
        t -= 2 * side;
        *x = -rr + (int64_t)t; *y = rr;
    } else {
        t -= 3 * side;
        *x = rr; *y = rr - (int64_t)t;
    }
}

static uint64_t *A316967_walk = 0;
static size_t A316967_cnt = 0;

static bool A316967_used(const uint64_t *tab, size_t mask, uint64_t key) {
    size_t h = (size_t)((key * 11400714819323198485ULL) >> 20) & mask;
    while (tab[h]) {
        if (tab[h] == key) return true;
        h = (h + 1) & mask;
    }
    return false;
}

static void A316967_insert(uint64_t *tab, size_t mask, uint64_t key) {
    size_t h = (size_t)((key * 11400714819323198485ULL) >> 20) & mask;
    while (tab[h]) h = (h + 1) & mask;
    tab[h] = key;
}

static void A316967_build(size_t total) {
    size_t tsz = 16;
    while (tsz < total * 4) tsz <<= 1;
    uint64_t *tab = (uint64_t *)calloc(tsz, sizeof(uint64_t));
    uint64_t *w = (uint64_t *)malloc(total * sizeof(uint64_t));
    if (!tab || !w) { free(tab); free(w); return; }
    size_t mask = tsz - 1;
    size_t cnt = 1;
    w[0] = 1;
    A316967_insert(tab, mask, 1);
    const int dx[4] = { 1, 1, -1, -1 };
    const int dy[4] = { 1, -1, 1, -1 };
    while (cnt < total) {
        int64_t cx, cy;
        A316967_coord_from_num(w[cnt - 1], &cx, &cy);
        uint64_t best = 0;
        for (int k = 0; k < 4; ++k) {
            uint64_t cand = A316967_num_from_coord(cx + dx[k], cy + dy[k]);
            if (A316967_used(tab, mask, cand)) continue;
            if (best == 0 || cand < best) best = cand;
        }
        if (best == 0) break;
        w[cnt++] = best;
        A316967_insert(tab, mask, best);
    }
    free(tab);
    free(A316967_walk);
    A316967_walk = w;
    A316967_cnt = cnt;
}

seq_t a(idx_t n) {
    if ((uint64_t)n >= A316967_cnt) {
        size_t want = (size_t)n + 1;
        size_t dbl = A316967_cnt * 2;
        if (want < dbl) want = dbl;
        if (want < 1024) want = 1024;
        A316967_build(want);
        if ((uint64_t)n >= A316967_cnt) return 0;
    }
    return (seq_t)(A316967_walk[n] - 1);
}
