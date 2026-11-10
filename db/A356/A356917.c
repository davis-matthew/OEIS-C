/*
 * A356917  Irregular triangle read by rows where row n lists the
 *          Colijn-Plazzotta subtree numbers, in ascending order, of each
 *          vertex of the rooted binary tree with their tree number n.
 * https://oeis.org/A356917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   row(1) = {1}; row(n) = sort {row(x), row(y), n} for n>=2, x =
 *            A002024(n-1), y = A002260(n-1); the code finds the row holding
 *            index n of the flattened triangle and returns its k-th entry
 * Range:     u8 n <= 5943, u16 n <= 13258, u32 n <= 13258, u64 n <= 13258,
 *            u128 n <= 13258, big any n
 * Validated: standard, pass, 107 terms through n = 9729 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void cp_xy(uint64_t n, uint64_t *x, uint64_t *y) {
    uint64_t p = n - 1;
    uint64_t k = 1;
    while (k * (k + 1) / 2 < p) k++;
    *x = k;
    *y = p - k * (k - 1) / 2;
}

static uint64_t *cp_L = NULL;
static uint64_t cp_cnt = 0, cp_cap = 0;

static void cp_extend(uint64_t n) {
    if (cp_cap < n + 1) {
        uint64_t nc = cp_cap ? cp_cap : 64;
        while (nc < n + 1) nc *= 2;
        cp_L = (uint64_t *)realloc(cp_L, nc * sizeof(uint64_t));
        cp_cap = nc;
    }
    if (cp_cnt == 0) { cp_L[1] = 1; cp_cnt = 1; }
    while (cp_cnt < n) {
        uint64_t m = cp_cnt + 1, x, y;
        cp_xy(m, &x, &y);
        cp_L[m] = cp_L[x] + cp_L[y] + 1;
        cp_cnt = m;
    }
}

static void cp_collect(uint64_t n, uint64_t *buf, uint64_t *pos) {
    if (n == 1) { buf[(*pos)++] = 1; return; }
    uint64_t x, y;
    cp_xy(n, &x, &y);
    cp_collect(x, buf, pos);
    cp_collect(y, buf, pos);
    buf[(*pos)++] = n;
}

seq_t a(idx_t m) {
    uint64_t cum = 0, n = 0, len = 0;
    while (1) {
        n++;
        cp_extend(n);
        len = cp_L[n];
        if ((uint64_t)m <= cum + len) break;
        cum += len;
    }
    uint64_t k = (uint64_t)m - cum; /* 1-based position in row */
    uint64_t *buf = (uint64_t *)malloc(len * sizeof(uint64_t));
    uint64_t pos = 0;
    cp_collect(n, buf, &pos);
    for (uint64_t i = 1; i < len; i++) {
        uint64_t v = buf[i], j = i;
        while (j > 0 && buf[j - 1] > v) { buf[j] = buf[j - 1]; j--; }
        buf[j] = v;
    }
    uint64_t r = buf[k - 1];
    free(buf);
    return (seq_t)r;
}
