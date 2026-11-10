/*
 * A286295  The right edge of the triangle in A274650.
 * https://oeis.org/A286295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A274650(n(n+3)/2), the diagonal T(n,n) of the triangle;
 *            computed by greedy construction with bitsets
 * Range:     u8 n <= 198, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 92 terms through n = 1000 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned *R286295_edge = 0;
static idx_t R286295_built = 0;

static void R286295_build(idx_t rows)
{
    size_t cap = (size_t)rows;
    size_t W = (4 * cap + 64) / 64 + 2;
    size_t nd = 2 * cap + 1;
    uint64_t *col = (uint64_t *)calloc(cap * W, sizeof(uint64_t));
    uint64_t *dg  = (uint64_t *)calloc(nd * W, sizeof(uint64_t));
    uint64_t *an  = (uint64_t *)calloc(nd * W, sizeof(uint64_t));
    uint64_t *rw  = (uint64_t *)calloc(W, sizeof(uint64_t));
    free(R286295_edge);
    R286295_edge = (unsigned *)calloc(cap, sizeof(unsigned));
    for (size_t r = 0; r < cap; ++r) {
        memset(rw, 0, W * sizeof(uint64_t));
        for (size_t k = 0; k <= r; ++k) {
            uint64_t *c = col + k * W;
            uint64_t *d = dg + (r - k + cap) * W;
            uint64_t *a = an + (r + k) * W;
            size_t v = 0;
            for (size_t w = 0; w < W; ++w) {
                uint64_t x = ~(rw[w] | c[w] | d[w] | a[w]);
                if (x) {
                    unsigned b = 0;
                    while (!((x >> b) & 1)) ++b;
                    v = w * 64 + b;
                    break;
                }
            }
            uint64_t m = (uint64_t)1 << (v & 63);
            rw[v >> 6] |= m;
            c[v >> 6] |= m;
            d[v >> 6] |= m;
            a[v >> 6] |= m;
            if (k == r) R286295_edge[r] = (unsigned)v;
        }
    }
    free(col); free(dg); free(an); free(rw);
    R286295_built = rows;
}

seq_t a(idx_t n)
{
    if (n >= R286295_built) {
        idx_t want = n + 1;
        if (want < 2 * R286295_built) want = 2 * R286295_built;
        if (want < 64) want = 64;
        R286295_build(want);
    }
    return (seq_t)R286295_edge[n];
}
