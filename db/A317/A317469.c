/*
 * A317469  Numbers missing from A317467.
 * https://oeis.org/A317469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer not visited by the (2,2)-leaper
 *            walk of A317467 (simulation of the walk, then scan for gaps)
 * Range:     u8 n <= 223, u16 n <= 13756, u32 n <= 13756, u64 n <= 13756,
 *            u128 n <= 13756, big any n
 * Validated: standard, pass, 87 terms through n = 9786 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t A317469_key(int32_t x, int32_t y)
{
    return ((((uint64_t)(uint32_t)x) << 32) | (uint64_t)(uint32_t)y) + 1ULL;
}

static uint64_t A317469_spiral(int32_t x, int32_t y)
{
    if (x == 0 && y == 0) return 1ULL;
    int32_t ax = x < 0 ? -x : x;
    int32_t ay = y < 0 ? -y : y;
    int32_t m = ax > ay ? ax : ay;
    uint64_t prev = (uint64_t)(2 * m - 1) * (uint64_t)(2 * m - 1);
    uint64_t off;
    if (x == m && y > -m) off = (uint64_t)(y + m);
    else if (y == m && x < m) off = (uint64_t)(2 * m + (m - x));
    else if (x == -m && y < m) off = (uint64_t)(4 * m + (m - y));
    else off = (uint64_t)(6 * m + (x + m));
    return prev + off;
}

static size_t A317469_slot(uint64_t key, size_t mask)
{
    return (size_t)((key * 11400714819323198485ULL) >> 20) & mask;
}

static int A317469_cmp(const void *p, const void *q)
{
    uint64_t u = *(const uint64_t *)p, v = *(const uint64_t *)q;
    return u < v ? -1 : (u > v ? 1 : 0);
}

seq_t a(idx_t n)
{
    size_t cap = (size_t)n * 2 + 1000;
    size_t tsz = 1;
    while (tsz < cap * 4) tsz <<= 1;
    size_t mask = tsz - 1;
    uint64_t *tab = (uint64_t *)calloc(tsz, sizeof(uint64_t));
    uint64_t *vals = (uint64_t *)malloc((cap + 2) * sizeof(uint64_t));
    size_t cnt = 0;
    int32_t cx = 0, cy = 0;
    {
        uint64_t k = A317469_key(0, 0);
        tab[A317469_slot(k, mask)] = k;
        vals[cnt++] = 1ULL;
    }
    while (cnt < cap) {
        int32_t dx[4] = {2, -2, 2, -2};
        int32_t dy[4] = {2, 2, -2, -2};
        uint64_t best = UINT64_MAX;
        int32_t bx = 0, by = 0;
        bool found = false;
        for (int i = 0; i < 4; i++) {
            int32_t nx = cx + dx[i], ny = cy + dy[i];
            uint64_t k = A317469_key(nx, ny);
            size_t s = A317469_slot(k, mask);
            bool vis = false;
            while (tab[s]) {
                if (tab[s] == k) { vis = true; break; }
                s = (s + 1) & mask;
            }
            if (vis) continue;
            uint64_t v = A317469_spiral(nx, ny);
            if (v < best) { best = v; bx = nx; by = ny; found = true; }
        }
        if (!found) break;
        cx = bx; cy = by;
        uint64_t k = A317469_key(cx, cy);
        size_t s = A317469_slot(k, mask);
        while (tab[s]) s = (s + 1) & mask;
        tab[s] = k;
        vals[cnt++] = best;
    }
    qsort(vals, cnt, sizeof(uint64_t), A317469_cmp);
    uint64_t m = 0, remaining = (uint64_t)n;
    size_t p = 0;
    while (remaining > 0) {
        m++;
        while (p < cnt && vals[p] < m) p++;
        if (p < cnt && vals[p] == m) continue;
        remaining--;
    }
    free(tab);
    free(vals);
    return (seq_t)m;
}
