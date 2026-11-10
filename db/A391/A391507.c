/*
 * A391507  Primitive coreful-bi-unitary abundant numbers: powerful numbers k
 *          for which A363334(k) > 2*k.
 * https://oeis.org/A391507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th powerful k (all prime exponents >= 2) with
 *            A363334(k) > 2*k, where A363334 is multiplicative with
 *            A363334(p^e) = sum_{j=1..e} p^j - (p^(e/2) if e even). Computed
 *            by DFS over powerful numbers up to a doubling limit, then sorted
 * Range:     u8 n <= 1, u16 n <= 72, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 9768 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *A391507_arr = 0;
static size_t A391507_cnt = 0, A391507_cap = 0;
static uint64_t A391507_limit = 0;
static uint64_t *A391507_pr = 0;
static size_t A391507_np = 0;

static void A391507_push(uint64_t v)
{
    if (A391507_cnt == A391507_cap) {
        A391507_cap = A391507_cap ? A391507_cap * 2 : 1024;
        A391507_arr = (uint64_t *)realloc(A391507_arr, A391507_cap * sizeof(uint64_t));
    }
    A391507_arr[A391507_cnt++] = v;
}

static void A391507_dfs(size_t idx, uint64_t cur, uint64_t A)
{
    for (size_t i = idx; i < A391507_np; ++i) {
        uint64_t p = A391507_pr[i];
        if (cur > A391507_limit / p / p) break;
        uint64_t pw = p * p;
        uint64_t sum = p + pw;      /* sum_{j=1..2} */
        uint64_t half = p;          /* p^(e/2) */
        unsigned e = 2;
        for (;;) {
            uint64_t val = sum - ((e & 1) ? 0 : half);
            uint64_t nc = cur * pw;
            uint64_t nA = A * val;
            if (nA > 2 * nc) A391507_push(nc);
            A391507_dfs(i + 1, nc, nA);
            if (pw > A391507_limit / p / cur) break;
            pw *= p;
            ++e;
            sum += pw;
            if ((e & 1) == 0) half *= p;
        }
    }
}

static int A391507_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return a < b ? -1 : (a > b);
}

static void A391507_build(uint64_t limit)
{
    uint64_t s = 1;
    while (s * s <= limit) ++s;
    size_t m = (size_t)s + 1;
    char *comp = (char *)calloc(m + 1, 1);
    free(A391507_pr);
    A391507_pr = (uint64_t *)malloc((m + 1) * sizeof(uint64_t));
    A391507_np = 0;
    for (size_t i = 2; i <= m; ++i) {
        if (!comp[i]) {
            A391507_pr[A391507_np++] = i;
            for (size_t j = i * i; j <= m; j += i) comp[j] = 1;
        }
    }
    free(comp);
    A391507_limit = limit;
    A391507_cnt = 0;
    A391507_dfs(0, 1, 1);
    qsort(A391507_arr, A391507_cnt, sizeof(uint64_t), A391507_cmp);
}

seq_t a(idx_t n)
{
    if (A391507_limit == 0) A391507_build(1000000ULL);
    while ((uint64_t)A391507_cnt < (uint64_t)n)
        A391507_build(A391507_limit * 4);
    return (seq_t)A391507_arr[n - 1];
}
