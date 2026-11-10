/*
 * A339696  Let G be the undirected graph with nodes {g_k, k >= 0} such that
 *          for any k >= 0, g_k is connected to g_{k+1} and g_{A006068(k)} is
 *          connected to g_{A006068(k+1)}; a(n) is the number of nodes at
 *          distance n from g_0.
 * https://oeis.org/A339696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   breadth-first search from g_0 on the infinite graph; neighbours
 *            of node u are u-1, u+1 and A006068(G(u)-1), A006068(G(u)+1),
 *            where G(u) = u XOR floor(u/2) is the inverse of A006068; a(n) =
 *            number of nodes first reached at level n
 * Range:     u8 n <= 305, u16 n <= 4000, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Validated: standard, pass, 86 terms through n = 3807 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* A006068: inverse Gray code (prefix XOR of shifts) */
static uint64_t A006068_g(uint64_t r)
{
    r ^= r >> 1;
    r ^= r >> 2;
    r ^= r >> 4;
    r ^= r >> 8;
    r ^= r >> 16;
    r ^= r >> 32;
    return r;
}

static uint64_t *A339696_H = NULL;   /* stores key+1, 0 = empty */
static size_t A339696_Hcap = 0, A339696_Hcnt = 0;
static int A339696_Hshift = 64;

static uint64_t *A339696_cur = NULL, *A339696_nxt = NULL;
static size_t A339696_curn = 0, A339696_nxtn = 0, A339696_nxtcap = 0, A339696_curcap = 0;
static uint64_t *A339696_cnt = NULL;
static size_t A339696_levels = 0, A339696_cntcap = 0;

static size_t A339696_slot(uint64_t key)
{
    return (size_t)((key * 0x9E3779B97F4A7C15ULL) >> A339696_Hshift);
}

static void A339696_rehash(size_t newcap, int newshift)
{
    uint64_t *old = A339696_H;
    size_t oldcap = A339696_Hcap, i;
    A339696_H = (uint64_t *)calloc(newcap, sizeof(uint64_t));
    A339696_Hcap = newcap;
    A339696_Hshift = newshift;
    for (i = 0; i < oldcap; i++) {
        if (old[i]) {
            size_t s = A339696_slot(old[i] - 1);
            while (A339696_H[s]) s = (s + 1) & (newcap - 1);
            A339696_H[s] = old[i];
        }
    }
    free(old);
}

/* returns 1 if newly inserted */
static int A339696_insert(uint64_t key)
{
    size_t s;
    if ((A339696_Hcnt + 1) * 2 > A339696_Hcap)
        A339696_rehash(A339696_Hcap * 2, A339696_Hshift - 1);
    s = A339696_slot(key);
    while (A339696_H[s]) {
        if (A339696_H[s] == key + 1) return 0;
        s = (s + 1) & (A339696_Hcap - 1);
    }
    A339696_H[s] = key + 1;
    A339696_Hcnt++;
    return 1;
}

static void A339696_push(uint64_t v)
{
    if (A339696_nxtn == A339696_nxtcap) {
        A339696_nxtcap = A339696_nxtcap ? A339696_nxtcap * 2 : 1024;
        A339696_nxt = (uint64_t *)realloc(A339696_nxt, A339696_nxtcap * sizeof(uint64_t));
    }
    A339696_nxt[A339696_nxtn++] = v;
}

static void A339696_visit(uint64_t v)
{
    if (A339696_insert(v)) A339696_push(v);
}

seq_t a(idx_t n)
{
    if (A339696_levels == 0) {
        A339696_Hcap = 1024;
        A339696_Hshift = 64 - 10;
        A339696_H = (uint64_t *)calloc(A339696_Hcap, sizeof(uint64_t));
        A339696_Hcnt = 0;
        A339696_insert(0);
        A339696_curcap = 1024;
        A339696_cur = (uint64_t *)malloc(A339696_curcap * sizeof(uint64_t));
        A339696_cur[0] = 0;
        A339696_curn = 1;
        A339696_cntcap = 1024;
        A339696_cnt = (uint64_t *)malloc(A339696_cntcap * sizeof(uint64_t));
        A339696_cnt[0] = 1;
        A339696_levels = 1;
    }
    while ((uint64_t)A339696_levels <= n) {
        size_t i;
        A339696_nxtn = 0;
        for (i = 0; i < A339696_curn; i++) {
            uint64_t u = A339696_cur[i];
            uint64_t j = u ^ (u >> 1);
            if (u > 0) A339696_visit(u - 1);
            A339696_visit(u + 1);
            if (j > 0) A339696_visit(A006068_g(j - 1));
            A339696_visit(A006068_g(j + 1));
        }
        if (A339696_levels == A339696_cntcap) {
            A339696_cntcap *= 2;
            A339696_cnt = (uint64_t *)realloc(A339696_cnt, A339696_cntcap * sizeof(uint64_t));
        }
        A339696_cnt[A339696_levels++] = (uint64_t)A339696_nxtn;
        {
            uint64_t *t = A339696_cur;
            size_t tc = A339696_curcap;
            A339696_cur = A339696_nxt;
            A339696_curcap = A339696_nxtcap;
            A339696_curn = A339696_nxtn;
            A339696_nxt = t;
            A339696_nxtcap = tc;
            A339696_nxtn = 0;
        }
    }
    return (seq_t)A339696_cnt[n];
}
