/*
 * A327000  A(n, k) = A309522(n, k) - A327001(n, k) for n >= 0 and k >= 3,
 *          square array read by ascending antidiagonals.
 * https://oeis.org/A327000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A309522(r,c) - A327001(r,c), read by antidiagonals with c
 *            >= 3: for d >= 0, i = 0..d the terms are (r,c) = (d-i, 3+i). For
 *            r = 0: A(0,c) = Catalan(c) - 2^(c-1). For r >= 1: A309522(r,c) =
 *            Sum_{v=0..c-1} C(r*c-1, r*v)*A(r,v)*A(r,c-1-v), A(r,0) = 1.
 *            A327001(r,c) = Sum_{j=0..c-1} C(r*c-1, r*j...
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 29, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-08)
 * Helpers:   binomial, catalan, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"
#include "../utils/mem.h"

static seq_t A327000_cell(idx_t r, idx_t c)
{
    if (r == 0) {
        seq_t p = 1;
        for (idx_t i = 1; i < c; ++i) p = p * 2;
        return catalan(c) - p;
    }
    size_t len = (size_t)c + 1;
    seq_t *B = mem_new(seq_t, len);
    seq_t *G = mem_new(seq_t, len);
    B[0] = 1;
    G[0] = 1;
    for (idx_t m = 1; m <= c; ++m) {
        seq_t sb = 0, sg = 0;
        for (idx_t v = 0; v < m; ++v) {
            seq_t bin = binomial(r * m - 1, r * v);
            sb += bin * B[(size_t)v] * B[(size_t)(m - 1 - v)];
            sg += bin * G[(size_t)v];
        }
        B[(size_t)m] = sb;
        G[(size_t)m] = sg;
    }
    seq_t res = B[(size_t)c] - G[(size_t)c];
    mem_free(B);
    mem_free(G);
    return res;
}

seq_t a(idx_t n)
{
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= n) ++d;
    idx_t i = n - (d * (d + 1)) / 2;
    idx_t r = d - i;
    idx_t c = 3 + i;
    return A327000_cell(r, c);
}
