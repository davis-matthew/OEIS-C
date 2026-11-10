/*
 * A284853  Positions of 1 in A284851; complement of A284852.
 * https://oeis.org/A284853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A026367(n); A026367: b(1)=1, b(2)=2; for n>2,
 *            b(n)=b(n-1)+2 if n=2*b(k)+1 for some k, else b(n)=b(n-1)+1
 * Range:     u8 n <= 93, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9722 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A284853_b = NULL;
static unsigned char *A284853_mark = NULL;
static size_t A284853_cap = 0;
static size_t A284853_cnt = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    size_t N = (size_t)n;
    if (N > A284853_cnt) {
        if (N + 1 > A284853_cap) {
            size_t nc = A284853_cap ? A284853_cap * 2 : 64;
            while (nc < N + 1) nc *= 2;
            if (!A284853_b) {
                A284853_b = mem_new(seq_t, nc);
            } else {
                seq_t *t = mem_resize(seq_t, A284853_b, nc);
                if (!t) abort();
                A284853_b = t;
            }
            if (!A284853_b) abort();
            size_t msz = 3 * nc + 8;
            unsigned char *m = (unsigned char *)realloc(A284853_mark, msz);
            if (!m) abort();
            size_t old = A284853_mark ? (3 * A284853_cap + 8) : 0;
            memset(m + old, 0, msz - old);
            A284853_mark = m;
            A284853_cap = nc;
        }
        if (A284853_cnt == 0) {
            A284853_b[1] = 1;
            A284853_mark[1] = 1;
            A284853_cnt = 1;
        }
        for (size_t i = A284853_cnt + 1; i <= N; ++i) {
            bool in = false;
            if (i & 1) {
                size_t m = (i - 1) / 2;
                if (A284853_mark[m]) in = true;
            }
            seq_t v = A284853_b[i - 1];
            v += in ? 2 : 1;
            A284853_b[i] = v;
            A284853_mark[(size_t)v] = 1;
        }
        A284853_cnt = N;
    }
    return (seq_t)2 * A284853_b[N];
}
