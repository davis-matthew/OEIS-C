/*
 * A348459  a(n) = max(A307720(n), A307720(n+1)).
 * https://oeis.org/A348459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(A307720(n), A307720(n+1)); A307720 is built by greedy
 *            search: each term is the smallest x such that the product prev*x
 *            has been used fewer than prev*x times
 * Range:     u8 n <= 24999, u16 n <= 25000, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 118 terms through n = 9422 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A307720_seq = NULL;
static size_t A307720_len = 0, A307720_cap = 0;
static seq_t *A307720_cnt = NULL;
static size_t A307720_ccap = 0;

static void A307720_grow_cnt(size_t need)
{
    size_t nc = A307720_ccap ? A307720_ccap : 16;
    while (nc <= need) nc <<= 1;
    seq_t *t = mem_new(seq_t, nc);
    for (size_t i = 0; i < A307720_ccap; ++i) t[i] = A307720_cnt[i];
    if (A307720_cnt) mem_free(A307720_cnt);
    A307720_cnt = t;
    A307720_ccap = nc;
}

static void A307720_grow_seq(void)
{
    size_t nc = A307720_cap ? A307720_cap << 1 : 16;
    seq_t *t = mem_new(seq_t, nc);
    for (size_t i = 0; i < A307720_len; ++i) t[i] = A307720_seq[i];
    if (A307720_seq) mem_free(A307720_seq);
    A307720_seq = t;
    A307720_cap = nc;
}

static seq_t A307720_get(idx_t n)
{
    if (A307720_len == 0) {
        A307720_grow_seq();
        A307720_grow_cnt(16);
        A307720_seq[0] = (seq_t)1;
        A307720_len = 1;
    }
    while ((idx_t)A307720_len < n) {
        seq_t prev = A307720_seq[A307720_len - 1];
        seq_t x = (seq_t)1;
        for (;;) {
            seq_t prod = prev * x;
            size_t pi = (size_t)(long long)prod;
            if (pi >= A307720_ccap) A307720_grow_cnt(pi);
            if (A307720_cnt[pi] < prod) {
                A307720_cnt[pi]++;
                if (A307720_len >= A307720_cap) A307720_grow_seq();
                A307720_seq[A307720_len++] = x;
                break;
            }
            x++;
        }
    }
    return A307720_seq[n - 1];
}

seq_t a(idx_t n)
{
    seq_t u = A307720_get(n + 1);
    seq_t v = A307720_get(n);
    return (u > v) ? u : v;
}
