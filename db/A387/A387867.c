/*
 * A387867  a(n) is the number of n-digit terms in A387866.
 * https://oeis.org/A387867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = 9*(10^(n-1) - 9^(n-1) - (n-1)*9^(n-2)) + sum_{i=1}^{n-1}
 *            sum_p F(i,p)*G(n-i,p), where G(m,p) = number of m-digit strings
 *            over 1..9 with digit product p, F(1,p) = G(1,p), F(i,p) = G(i,p)
 *            - G(i-1,p) for i >= 2 (counting by DP over prime exponents of
 *            2,3,5,7)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static const int A387867_ex[10][4] = {
    {0,0,0,0},{0,0,0,0},{1,0,0,0},{0,1,0,0},{2,0,0,0},
    {0,0,1,0},{1,1,0,0},{0,0,0,1},{3,0,0,0},{0,2,0,0}
};

static size_t A387867_idx(int k, int a, int b, int c, int d)
{
    size_t B = (size_t)(2 * k + 1), C = (size_t)(k + 1), D = (size_t)(k + 1);
    return ((((size_t)a * B + (size_t)b) * C + (size_t)c) * D + (size_t)d);
}

static size_t A387867_size(int k)
{
    return (size_t)(3 * k + 1) * (size_t)(2 * k + 1) * (size_t)(k + 1) * (size_t)(k + 1);
}

extern seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    int N = (int)n - 1;

    /* zero-containing part */
    seq_t p10 = 1, p9a = 1, p9b = 1;   /* 10^(n-1), 9^(n-1), 9^(n-2) */
    for (int i = 0; i < N; ++i) p10 = p10 * 10;
    for (int i = 0; i < N; ++i) p9a = p9a * 9;
    for (int i = 0; i < N - 1; ++i) p9b = p9b * 9;
    seq_t total = ((seq_t)9) * (p10 - p9a - (seq_t)(N) * p9b);

    seq_t **G = (seq_t **)malloc(sizeof(seq_t *) * (size_t)(N + 1));
    G[0] = mem_new(seq_t, A387867_size(0));
    G[0][0] = 1;
    for (int k = 1; k <= N; ++k) {
        G[k] = mem_new(seq_t, A387867_size(k));
        int pk = k - 1;
        for (int ea = 0; ea <= 3 * pk; ++ea)
            for (int eb = 0; eb <= 2 * pk; ++eb)
                for (int ec = 0; ec <= pk; ++ec)
                    for (int ed = 0; ed <= pk; ++ed) {
                        seq_t v = G[pk][A387867_idx(pk, ea, eb, ec, ed)];
                        if (v == 0) continue;
                        for (int dg = 1; dg <= 9; ++dg) {
                            size_t t = A387867_idx(k, ea + A387867_ex[dg][0],
                                                   eb + A387867_ex[dg][1],
                                                   ec + A387867_ex[dg][2],
                                                   ed + A387867_ex[dg][3]);
                            G[k][t] = G[k][t] + v;
                        }
                    }
    }

    for (int i = 1; i <= N; ++i) {
        int m = (int)n - i;
        for (int ea = 0; ea <= 3 * i; ++ea)
            for (int eb = 0; eb <= 2 * i; ++eb)
                for (int ec = 0; ec <= i; ++ec)
                    for (int ed = 0; ed <= i; ++ed) {
                        seq_t f = G[i][A387867_idx(i, ea, eb, ec, ed)];
                        if (f == 0) continue;
                        if (i >= 2) {
                            int q = i - 1;
                            if (ea <= 3 * q && eb <= 2 * q && ec <= q && ed <= q)
                                f = f - G[q][A387867_idx(q, ea, eb, ec, ed)];
                        }
                        if (f == 0) continue;
                        if (ea <= 3 * m && eb <= 2 * m && ec <= m && ed <= m)
                            total = total + f * G[m][A387867_idx(m, ea, eb, ec, ed)];
                    }
    }

    for (int k = 0; k <= N; ++k) mem_free(G[k]);
    free(G);
    return total;
}
