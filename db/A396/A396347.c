/*
 * A396347  Least k such that A396346(k) = n.
 * https://oeis.org/A396347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that A396346(k) = n, found by simulating
 *            A396346: b(0)=0; b(k)=b(k-1)-k if >=0 and not already in the
 *            sequence, else b(k-1)+k-1. First-occurrence indices are kept in
 *            a hash table
 * Range:     u8 n <= 32, u16 n <= 9937, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 8627 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct { uint64_t key; uint64_t idx; bool used; } A396347_ent;

static A396347_ent *A396347_tab = NULL;
static size_t A396347_cap = 0, A396347_cnt = 0;
static uint64_t A396347_i = 0;
static seq_t A396347_prev;
static bool A396347_init = false;

static size_t A396347_h(uint64_t k, size_t cap)
{
    k ^= k >> 33; k *= 0xff51afd7ed558ccdULL;
    k ^= k >> 33; k *= 0xc4ceb9fe1a85ec53ULL;
    k ^= k >> 33;
    return (size_t)(k & (cap - 1));
}

static A396347_ent *A396347_find(uint64_t key)
{
    size_t p = A396347_h(key, A396347_cap);
    while (A396347_tab[p].used) {
        if (A396347_tab[p].key == key) return &A396347_tab[p];
        p = (p + 1) & (A396347_cap - 1);
    }
    return NULL;
}

static void A396347_put(uint64_t key, uint64_t idx)
{
    if ((A396347_cnt + 1) * 2 > A396347_cap) {
        size_t nc = A396347_cap ? A396347_cap * 2 : 1024;
        A396347_ent *nt = (A396347_ent *)calloc(nc, sizeof(A396347_ent));
        if (!nt) exit(EXIT_FAILURE);
        for (size_t j = 0; j < A396347_cap; ++j)
            if (A396347_tab[j].used) {
                size_t p = A396347_h(A396347_tab[j].key, nc);
                while (nt[p].used) p = (p + 1) & (nc - 1);
                nt[p] = A396347_tab[j];
            }
        free(A396347_tab);
        A396347_tab = nt;
        A396347_cap = nc;
    }
    size_t p = A396347_h(key, A396347_cap);
    while (A396347_tab[p].used) {
        if (A396347_tab[p].key == key) return;
        p = (p + 1) & (A396347_cap - 1);
    }
    A396347_tab[p].key = key;
    A396347_tab[p].idx = idx;
    A396347_tab[p].used = true;
    A396347_cnt++;
}

seq_t a(idx_t n)
{
    if (!A396347_init) {
        A396347_init = true;
        A396347_prev = 0;
        A396347_i = 0;
        A396347_put(0, 0);
    }
    uint64_t key = (uint64_t)n;
    for (;;) {
        A396347_ent *e = A396347_find(key);
        if (e) return (seq_t)e->idx;
        A396347_i++;
        seq_t k = (seq_t)A396347_i;
        seq_t nv;
        bool ok = false;
        if (A396347_prev >= k) {
            nv = A396347_prev - k;
            ok = (A396347_find((uint64_t)(unsigned long long)nv) == NULL);
        }
        if (!ok) nv = A396347_prev + k - 1;
        A396347_prev = nv;
        A396347_put((uint64_t)(unsigned long long)nv, A396347_i);
    }
}
