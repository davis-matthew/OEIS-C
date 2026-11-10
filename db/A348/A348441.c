/*
 * A348441  Indices of records in A088177.
 * https://oeis.org/A348441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of n-th strict record (left-to-right maximum) of
 *            A088177; computed by generating A088177 (a(1)=a(2)=1, then
 *            smallest positive value making all adjacent products distinct)
 * Range:     u8 n <= 12, u16 n <= 183, u32 n <= 381, u64 n <= 381,
 *            u128 n <= 381, big any n
 * Validated: standard, pass, 73 terms through n = 380 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static inline size_t A348441_hash(seq_t x)
{
    uint64_t h = (uint64_t)x;
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccdULL;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53ULL;
    h ^= h >> 33;
    return (size_t)h;
}

static seq_t *A348441_tab = NULL;
static size_t A348441_mask = 0;
static size_t A348441_cnt = 0;

static bool A348441_contains(seq_t v)
{
    size_t pos = A348441_hash(v) & A348441_mask;
    while (A348441_tab[pos]) {
        if (A348441_tab[pos] == v) return true;
        pos = (pos + 1) & A348441_mask;
    }
    return false;
}

static void A348441_insert(seq_t v)
{
    if ((A348441_cnt + 1) * 10 > (A348441_mask + 1) * 7) {
        size_t oc = A348441_mask + 1;
        size_t nc = oc << 1;
        size_t nm = nc - 1;
        seq_t *nt = mem_new(seq_t, nc);
        if (!nt) abort();
        for (size_t i = 0; i < oc; ++i) {
            seq_t w = A348441_tab[i];
            if (!w) continue;
            size_t p = A348441_hash(w) & nm;
            while (nt[p]) p = (p + 1) & nm;
            nt[p] = w;
        }
        mem_free(A348441_tab);
        A348441_tab = nt;
        A348441_mask = nm;
    }
    size_t pos = A348441_hash(v) & A348441_mask;
    while (A348441_tab[pos]) pos = (pos + 1) & A348441_mask;
    A348441_tab[pos] = v;
    ++A348441_cnt;
}

static idx_t *A348441_rec = NULL;
static size_t A348441_rcnt = 0;
static size_t A348441_rcap = 0;
static idx_t A348441_len = 0;
static seq_t A348441_prev = 0;
static seq_t A348441_max = 0;

static void A348441_push(idx_t i)
{
    if (A348441_rcnt == A348441_rcap) {
        size_t nc = A348441_rcap ? A348441_rcap * 2 : 64;
        idx_t *p = (idx_t *)realloc(A348441_rec, nc * sizeof(idx_t));
        if (!p) abort();
        A348441_rec = p;
        A348441_rcap = nc;
    }
    A348441_rec[A348441_rcnt++] = i;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!A348441_tab) {
        A348441_mask = 15;
        A348441_tab = mem_new(seq_t, A348441_mask + 1);
        if (!A348441_tab) abort();
        A348441_cnt = 0;
        /* a(1)=1 (record at index 1), a(2)=1 */
        A348441_push(1);
        A348441_max = 1;
        A348441_prev = 1;
        A348441_len = 2;
        A348441_insert((seq_t)1);
    }
    while (A348441_rcnt < n) {
        seq_t cur = 1;
        seq_t prod;
        for (;;) {
            prod = A348441_prev * cur;
            if (!A348441_contains(prod)) break;
            ++cur;
        }
        A348441_insert(prod);
        ++A348441_len;
        A348441_prev = cur;
        if (cur > A348441_max) {
            A348441_max = cur;
            A348441_push(A348441_len);
        }
    }
    return (seq_t)A348441_rec[n - 1];
}
