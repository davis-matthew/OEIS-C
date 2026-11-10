/*
 * A369541  Numbers k neither squarefree nor prime powers that are products of
 *          primorials such that A119288(k) <= k/A007947(k) < A053669(k).
 * https://oeis.org/A369541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m * P(r), where P(r) is the r-th primorial (r >= 2) and m
 *            runs over the numbers of A025487 with 3 <= m < prime(r+1), taken
 *            in increasing order of r and then of m. Found by enumerating
 *            those m row by row
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 30, u64 n <= 81, u128 n <= 209,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 9741 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const uint64_t A369541_sp[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
static uint64_t A369541_buf[100000];
static size_t A369541_cnt;

static void A369541_dfs(int pi, int maxe, uint64_t cur, uint64_t limit)
{
    if (cur >= 3 && cur < limit && A369541_cnt < 100000)
        A369541_buf[A369541_cnt++] = cur;
    if (pi >= 16) return;
    for (int e = 1; e <= maxe; e++) {
        if (cur > (limit - 1) / A369541_sp[pi]) break;
        cur *= A369541_sp[pi];
        if (cur >= limit) break;
        A369541_dfs(pi + 1, e, cur, limit);
    }
}

static int A369541_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return a < b ? -1 : (a > b ? 1 : 0);
}

static uint64_t A369541_nextprime(uint64_t p)
{
    for (uint64_t c = p + 1;; c++) {
        int ok = 1;
        for (uint64_t d = 2; d * d <= c; d++)
            if (c % d == 0) { ok = 0; break; }
        if (ok) return c;
    }
}

seq_t a(idx_t n)
{
    seq_t P = 6;
    uint64_t q = 5;
    idx_t rem = n;
    for (;;) {
        A369541_cnt = 0;
        A369541_dfs(0, 64, 1, q);
        if (rem <= (idx_t)A369541_cnt) {
            qsort(A369541_buf, A369541_cnt, sizeof(uint64_t), A369541_cmp);
            return (seq_t)A369541_buf[rem - 1] * P;
        }
        rem -= (idx_t)A369541_cnt;
        P = P * (seq_t)q;
        q = A369541_nextprime(q);
    }
}
