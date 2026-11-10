/*
 * A297160  Positions of records in A220400.
 * https://oeis.org/A297160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>=2, a(n) = 4*N where N is the (n-1)th record
 *            position of ceil(d(N)/2), i.e. the record positions of A220400
 *            (n=4N, count = ceil(d(N)/2)). Computed by enumerating numbers
 *            with non-increasing prime exponents (A055932) up to a growing
 *            limit and scanning for records
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct { uint64_t N; uint64_t d; } A297160_node;

static A297160_node *A297160_buf = 0;
static size_t A297160_cnt = 0, A297160_cap = 0;
static uint64_t A297160_limit;
static const uint64_t A297160_pr[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};

static void A297160_push(uint64_t N, uint64_t d)
{
    if (A297160_cnt == A297160_cap) {
        A297160_cap = A297160_cap ? A297160_cap * 2 : 1024;
        A297160_buf = (A297160_node *)realloc(A297160_buf, A297160_cap * sizeof(A297160_node));
    }
    A297160_buf[A297160_cnt].N = N;
    A297160_buf[A297160_cnt].d = d;
    A297160_cnt++;
}

static void A297160_dfs(int idx, uint64_t N, int maxe, uint64_t d)
{
    A297160_push(N, d);
    if (idx >= 16) return;
    uint64_t p = A297160_pr[idx];
    uint64_t cur = N;
    for (int e = 1; e <= maxe; e++) {
        if (cur > A297160_limit / p) break;
        cur *= p;
        A297160_dfs(idx + 1, cur, e, d * (uint64_t)(e + 1));
    }
}

static int A297160_cmp(const void *x, const void *y)
{
    uint64_t a = ((const A297160_node *)x)->N, b = ((const A297160_node *)y)->N;
    return a < b ? -1 : (a > b ? 1 : 0);
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    idx_t want = n - 1;
    uint64_t L = (uint64_t)1 << 16;
    for (;;) {
        A297160_limit = L;
        A297160_cnt = 0;
        A297160_dfs(0, 1, 70, 1);
        qsort(A297160_buf, A297160_cnt, sizeof(A297160_node), A297160_cmp);
        uint64_t best = 0;
        idx_t found = 0;
        for (size_t i = 0; i < A297160_cnt; i++) {
            uint64_t f = (A297160_buf[i].d + 1) / 2;
            if (f > best) {
                best = f;
                found++;
                if (found == want) {
                    seq_t r = (seq_t)A297160_buf[i].N * 4;
                    free(A297160_buf);
                    A297160_buf = 0; A297160_cap = 0; A297160_cnt = 0;
                    return r;
                }
            }
        }
        if (L >= ((uint64_t)1 << 59)) break;
        L <<= 4;
    }
    free(A297160_buf);
    A297160_buf = 0; A297160_cap = 0; A297160_cnt = 0;
    return 0;
}
