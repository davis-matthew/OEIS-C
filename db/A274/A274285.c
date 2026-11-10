/*
 * A274285  Numbers that are a product of distinct numbers in A013655.
 * https://oeis.org/A274285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th smallest number that is a product of distinct terms
 *            of A013655 (2,5,7,12,19,...), computed by DFS over subset
 *            products below a doubling limit, then sort and dedupe
 * Range:     u8 n <= 30, u16 n <= 310, u32 n <= 1817, u64 n <= 1817,
 *            u128 n <= 1817, big any n
 * Validated: standard, pass, 72 terms through n = 1810 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A274285_buf = 0;
static size_t A274285_cnt = 0, A274285_cap = 0;
static uint64_t A274285_f[100];
static int A274285_nf = 0;
static uint64_t A274285_L = 0;
static uint64_t *A274285_res = 0;
static size_t A274285_rescnt = 0;

static void A274285_push(uint64_t v)
{
    if (A274285_cnt == A274285_cap) {
        A274285_cap = A274285_cap ? A274285_cap * 2 : 4096;
        A274285_buf = (uint64_t *)realloc(A274285_buf, A274285_cap * sizeof(uint64_t));
    }
    A274285_buf[A274285_cnt++] = v;
}

static void A274285_dfs(int start, uint64_t prod)
{
    for (int i = start; i < A274285_nf; i++) {
        if (A274285_f[i] > A274285_L / prod) break;
        uint64_t p = prod * A274285_f[i];
        A274285_push(p);
        A274285_dfs(i + 1, p);
    }
}

static int A274285_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return a < b ? -1 : (a > b ? 1 : 0);
}

static void A274285_build(uint64_t L)
{
    A274285_L = L;
    A274285_nf = 0;
    uint64_t x = 2, y = 5;
    A274285_f[A274285_nf++] = x;
    while (y <= L && A274285_nf < 100) {
        A274285_f[A274285_nf++] = y;
        uint64_t t = x + y;
        x = y;
        y = t;
    }
    A274285_cnt = 0;
    A274285_dfs(0, 1);
    qsort(A274285_buf, A274285_cnt, sizeof(uint64_t), A274285_cmp);
    size_t m = 0;
    for (size_t i = 0; i < A274285_cnt; i++) {
        if (m == 0 || A274285_buf[m - 1] != A274285_buf[i])
            A274285_buf[m++] = A274285_buf[i];
    }
    A274285_rescnt = m;
}

seq_t a(idx_t n)
{
    uint64_t L = A274285_L ? A274285_L : 1024;
    while (A274285_L == 0 || (uint64_t)n > A274285_rescnt) {
        A274285_build(L);
        if ((uint64_t)n <= A274285_rescnt) break;
        L *= 2;
    }
    return (seq_t)A274285_buf[n - 1];
}
