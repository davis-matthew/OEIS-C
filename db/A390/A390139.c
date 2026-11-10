/*
 * A390139  Integers k such that A008472(k) / A001222(k) = 1/2.
 * https://oeis.org/A390139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (increasing) with 2*A008472(k) = A001222(k); found
 *            by DFS over prime-power products up to a growing limit, then
 *            sorted
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static uint64_t *A390139_buf = 0;
static size_t A390139_cnt = 0, A390139_cap = 0;
static uint64_t A390139_L = 0;
static const unsigned A390139_P[] = {2,3,5,7,11,13,17,19,23,29,31};

static void A390139_push(uint64_t v)
{
    if (A390139_cnt == A390139_cap) {
        A390139_cap = A390139_cap ? A390139_cap * 2 : 1024;
        A390139_buf = (uint64_t *)realloc(A390139_buf, A390139_cap * sizeof(uint64_t));
    }
    A390139_buf[A390139_cnt++] = v;
}

static void A390139_dfs(int start, uint64_t v, unsigned s, unsigned E)
{
    for (int i = start; i < 11; i++) {
        unsigned p = A390139_P[i];
        unsigned s2 = s + p;
        if (s2 > 31) break;
        uint64_t w = v;
        unsigned e = 0;
        while (w <= A390139_L / p) {
            w *= p;
            e++;
            if (E + e == 2 * s2) A390139_push(w);
            A390139_dfs(i + 1, w, s2, E + e);
        }
    }
}

static int A390139_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return a < b ? -1 : (a > b);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A390139_L == 0 || (A390139_cnt < (size_t)n && A390139_L < ((uint64_t)1 << 62))) {
        A390139_L = A390139_L ? A390139_L * 4 : ((uint64_t)1 << 24);
        A390139_cnt = 0;
        A390139_dfs(0, 1, 0, 0);
        qsort(A390139_buf, A390139_cnt, sizeof(uint64_t), A390139_cmp);
    }
    if ((size_t)n > A390139_cnt) return 0;
    return (seq_t)A390139_buf[n - 1];
}
