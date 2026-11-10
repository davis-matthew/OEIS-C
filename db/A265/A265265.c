/*
 * A265265  Self-convolution of A265264.
 * https://oeis.org/A265265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} g(n-k)*g(k), where g = A265264: g(0)=1,
 *            g(1)=1, g(2m)=g(m)*b(m), g(2m+1)=g(m+1)*b(m), b(m)=Sum_{k=0..m}
 *            g(k)*g(m-k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 2994 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A265265_g = NULL;
static seq_t *A265265_b = NULL;
static idx_t A265265_cap = 0;
static idx_t A265265_cnt = 0; /* g[0..cnt-1] computed */

seq_t a(idx_t n)
{
    if (n + 1 > A265265_cap) {
        idx_t nc = (A265265_cap == 0) ? 16 : A265265_cap;
        while (nc < n + 1) nc <<= 1;
        A265265_g = mem_resize(seq_t, A265265_g, (nc + 2));
        A265265_b = mem_resize(seq_t, A265265_b, (nc / 2 + 2));
        mem_set(A265265_g + A265265_cap, 0, (nc + 2 - A265265_cap) * sizeof(seq_t));
        idx_t ob = (A265265_cap == 0) ? 0 : A265265_cap / 2 + 2;
        mem_set(A265265_b + ob, 0, (nc / 2 + 2 - ob) * sizeof(seq_t));
        A265265_cap = nc;
    }
    if (A265265_cnt == 0) {
        A265265_g[0] = 1;
        A265265_g[1] = 1;
        A265265_cnt = 2;
    }
    for (idx_t i = A265265_cnt; i <= n; ++i) {
        idx_t m = i >> 1;
        if ((i & 1) == 0) {
            seq_t s = 0;
            for (idx_t k = 0; k <= m; ++k)
                s += A265265_g[k] * A265265_g[m - k];
            A265265_b[m] = s;
            A265265_g[i] = A265265_g[m] * s;
        } else {
            A265265_g[i] = A265265_g[m + 1] * A265265_b[m];
        }
        A265265_cnt = i + 1;
    }
    seq_t r = 0;
    for (idx_t k = 0; k <= n; ++k)
        r += A265265_g[k] * A265265_g[n - k];
    return r;
}
