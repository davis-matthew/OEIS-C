/*
 * A276575  After a(0)=0, the first differences of A276573.
 * https://oeis.org/A276575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = A276573(n) - A276573(n-1), where A276573 is the
 *            infinite trunk of the tree defined by parent(x) = x -
 *            A002828(x). Method: compute levels (A260731) up to a bound M.
 *            Every infinite path must pass through the window [M-3, M]. When
 *            all window paths reach the same node at level n, tha...
 * Range:     u8 n <= 10028, u16 n <= 10028, u32 n <= 10028, u64 n <= 10028,
 *            u128 n <= 10028, big any n
 * Validated: standard, pass, 137 terms through n = 8815 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int s276575_two_squares(uint64_t m) {
    uint64_t p;
    while (m % 2 == 0) m /= 2;
    for (p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            if (p % 4 == 3 && (e & 1)) return 0;
        }
    }
    if (m > 1 && m % 4 == 3) return 0;
    return 1;
}

static int s276575_issq(uint64_t m) {
    uint64_t lo = 0, hi = 4294967296ULL;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (mid * mid < m) lo = mid + 1; else hi = mid;
    }
    return lo * lo == m;
}

static uint64_t s276575_least(uint64_t m) {
    if (m == 0) return 0;
    if (s276575_issq(m)) return 1;
    if (s276575_two_squares(m)) return 2;
    while (m % 4 == 0) m /= 4;
    if (m % 8 == 7) return 4;
    return 3;
}

static uint64_t *s276575_tr = NULL;
static size_t s276575_trlen = 0;

static int s276575_build(size_t n) {
    size_t M = 3 * n + 1000;
    for (;;) {
        uint32_t *lev = (uint32_t *)malloc((M + 1) * sizeof(uint32_t));
        uint64_t *par = (uint64_t *)malloc((M + 1) * sizeof(uint64_t));
        size_t x;
        int ok = 1;
        uint64_t node = 0;
        if (!lev || !par) { free(lev); free(par); return 0; }
        lev[0] = 0; par[0] = 0;
        for (x = 1; x <= M; x++) {
            par[x] = (uint64_t)x - s276575_least((uint64_t)x);
            lev[x] = lev[par[x]] + 1;
        }
        for (x = M - 3; x <= M && ok; x++) {
            uint64_t y = (uint64_t)x;
            if ((size_t)lev[y] < n + 1) { ok = 0; break; }
            while ((size_t)lev[y] > n) y = par[y];
            if (x == M - 3) node = y;
            else if (y != node) ok = 0;
        }
        if (ok) {
            uint64_t *t = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
            uint64_t y = node;
            if (!t) { free(lev); free(par); return 0; }
            for (;;) {
                t[lev[y]] = y;
                if (y == 0) break;
                y = par[y];
            }
            free(s276575_tr);
            s276575_tr = t;
            s276575_trlen = n + 1;
            free(lev); free(par);
            return 1;
        }
        free(lev); free(par);
        M *= 2;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if ((size_t)n >= s276575_trlen) {
        size_t want = (size_t)n;
        if (want < 2 * s276575_trlen) want = 2 * s276575_trlen;
        if (!s276575_build(want)) return (seq_t)0;
    }
    return (seq_t)(s276575_tr[n] - s276575_tr[n - 1]);
}
