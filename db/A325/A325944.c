/*
 * A325944  Least k such that A296062(k) = n.
 * https://oeis.org/A325944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; otherwise a breadth-first search over k in increasing
 *            order. * With f = A296062, the pair (f(m-1), f(m)) for m = 2j
 *            and m = 2j+1 follows from * (x, y) = (f(j-1), f(j)): * m = 2j ->
 *            (2x, x+y+1) * m = 2j+1 -> (x+y+1, 2y) * starting at m = 1 with
 *            (0, 0). Every child has a larger maximum co...
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 32, u64 n <= 92, u128 n <= 132,
 *            big any n
 * Validated: standard, pass, 67 terms through n = 8156 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *A325944_px = NULL, *A325944_py = NULL, *A325944_par = NULL;
static size_t A325944_cnt = 0, A325944_cap = 0;
static uint32_t *A325944_first = NULL;
static uint8_t *A325944_vis = NULL;
static idx_t A325944_N = 0;

static int A325944_push(uint32_t x, uint32_t y, uint32_t par, uint32_t W)
{
    size_t id = (size_t)y * W + x;
    if (A325944_vis[id >> 3] & (uint8_t)(1u << (id & 7))) return 1;
    A325944_vis[id >> 3] |= (uint8_t)(1u << (id & 7));
    if (A325944_cnt == A325944_cap) {
        size_t nc = A325944_cap ? A325944_cap * 2 : 1024;
        uint32_t *a1 = (uint32_t *)realloc(A325944_px, nc * sizeof(uint32_t));
        if (!a1) return 0;
        A325944_px = a1;
        a1 = (uint32_t *)realloc(A325944_py, nc * sizeof(uint32_t));
        if (!a1) return 0;
        A325944_py = a1;
        a1 = (uint32_t *)realloc(A325944_par, nc * sizeof(uint32_t));
        if (!a1) return 0;
        A325944_par = a1;
        A325944_cap = nc;
    }
    A325944_px[A325944_cnt] = x;
    A325944_py[A325944_cnt] = y;
    A325944_par[A325944_cnt] = par;
    if (A325944_first[y] == UINT32_MAX) A325944_first[y] = (uint32_t)A325944_cnt;
    A325944_cnt++;
    return 1;
}

static int A325944_build(idx_t Nreq)
{
    uint32_t N = (uint32_t)Nreq;
    uint32_t INF = N + 1, W = N + 2;
    size_t bits = (size_t)W * (size_t)(N + 1);
    size_t i;

    free(A325944_vis);
    free(A325944_first);
    A325944_vis = (uint8_t *)calloc(bits / 8 + 1, 1);
    A325944_first = (uint32_t *)malloc(((size_t)N + 1) * sizeof(uint32_t));
    A325944_N = 0;
    A325944_cnt = 0;
    if (!A325944_vis || !A325944_first) return 0;
    for (i = 0; i <= N; i++) A325944_first[i] = UINT32_MAX;

    if (!A325944_push(0, 0, 0, W)) return 0;          /* m = 1 : (f(0), f(1)) */
    for (i = 0; i < A325944_cnt; i++) {
        uint32_t x = A325944_px[i], y = A325944_py[i];
        uint32_t x0, y0, x1, y1;
        /* bit 0: m -> 2m */
        if (x == INF) { x0 = INF; y0 = INF; }
        else {
            x0 = 2 * x; if (x0 > N) x0 = INF;
            y0 = x + y + 1;
        }
        if (y0 <= N)
            if (!A325944_push(x0, y0, (uint32_t)(i << 1), W)) return 0;
        /* bit 1: m -> 2m+1 */
        if (x == INF) x1 = INF;
        else { x1 = x + y + 1; if (x1 > N) x1 = INF; }
        y1 = 2 * y;
        if (y1 <= N)
            if (!A325944_push(x1, y1, (uint32_t)((i << 1) | 1), W)) return 0;
    }
    free(A325944_vis);
    A325944_vis = NULL;
    A325944_N = Nreq;
    return 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > A325944_N) {
        idx_t N = A325944_N * 2;
        if (N < 64) N = 64;
        if (N < n) N = n;
        if (!A325944_build(N)) return 0;
    }
    {
        uint32_t node = A325944_first[n];
        size_t len = 0, cap = 256;
        uint8_t *bitsbuf;
        seq_t m = 1;
        if (node == UINT32_MAX) return 0;
        bitsbuf = (uint8_t *)malloc(cap);
        if (!bitsbuf) return 0;
        while (node != 0) {
            uint32_t p = A325944_par[node];
            if (len == cap) {
                uint8_t *t = (uint8_t *)realloc(bitsbuf, cap * 2);
                if (!t) { free(bitsbuf); return 0; }
                bitsbuf = t;
                cap *= 2;
            }
            bitsbuf[len++] = (uint8_t)(p & 1u);
            node = p >> 1;
        }
        while (len > 0) {
            len--;
            m = m * 2 + (seq_t)bitsbuf[len];
        }
        free(bitsbuf);
        return m;
    }
}
