/*
 * A366601  a(0) = 0. For a(n-1) a novel term a(n) = a(A025480(a(n-1))), and
 *          if a(n-1) has occurred k (>1) times, a(n) = k-1 (the number of
 *          repetitions of a(n-1) in all prior terms).
 * https://oeis.org/A366601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; if a(n-1) is novel, a(n) = a(A025480(a(n-1)));
 *            otherwise a(n) = k-1 where k = occurrences of a(n-1) so far;
 *            computed by simulation
 * Range:     u8 n <= 3392, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9309 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A366601_seq = 0;
static uint64_t *A366601_cnt = 0;
static uint64_t A366601_len = 0, A366601_cap = 0, A366601_ccap = 0;

static uint64_t A366601_A025480(uint64_t n)
{
    while (n & 1ULL)
        n >>= 1;
    return n >> 1;
}

static void A366601_push(uint64_t v)
{
    if (A366601_len >= A366601_cap) {
        uint64_t nc = A366601_cap ? A366601_cap * 2 : 1024;
        A366601_seq = (uint64_t *)realloc(A366601_seq, nc * sizeof(uint64_t));
        A366601_cap = nc;
    }
    if (v >= A366601_ccap) {
        uint64_t nc = A366601_ccap ? A366601_ccap : 1024;
        while (nc <= v) nc *= 2;
        A366601_cnt = (uint64_t *)realloc(A366601_cnt, nc * sizeof(uint64_t));
        for (uint64_t i = A366601_ccap; i < nc; i++) A366601_cnt[i] = 0;
        A366601_ccap = nc;
    }
    A366601_seq[A366601_len++] = v;
    A366601_cnt[v]++;
}

seq_t a(idx_t n)
{
    if (A366601_len == 0)
        A366601_push(0);
    while (A366601_len <= (uint64_t)n) {
        uint64_t last = A366601_seq[A366601_len - 1];
        uint64_t k = A366601_cnt[last];
        uint64_t nxt;
        if (k == 1)
            nxt = A366601_seq[A366601_A025480(last)];
        else
            nxt = k - 1;
        A366601_push(nxt);
    }
    return (seq_t)A366601_seq[n];
}
