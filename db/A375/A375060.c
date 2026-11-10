/*
 * A375060  Values of k for which A374966(k) = 0.
 * https://oeis.org/A375060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k-th index with A374966(k)=0 (a(1)=0); computed by
 *            simulating A374966: A374966(0)=0, A374966(k)=0 if k appeared
 *            earlier as a term, else A374966(k-1)+k
 * Range:     u8 n <= 37, u16 n <= 8943, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9927 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static unsigned char *zs_seen = NULL;
static size_t zs_cap = 0;
static uint64_t *zs_list = NULL;
static size_t zs_cnt = 0, zs_lcap = 0;
static uint64_t zs_k = 0, zs_cur = 0;
static int zs_init = 0;

static void zs_mark(uint64_t v)
{
    if (v >= zs_cap) {
        size_t nc = zs_cap ? zs_cap : 1024;
        while (nc <= v) nc <<= 1;
        unsigned char *t = (unsigned char *)realloc(zs_seen, nc);
        if (!t) abort();
        memset(t + zs_cap, 0, nc - zs_cap);
        zs_seen = t;
        zs_cap = nc;
    }
    zs_seen[v] = 1;
}

static void zs_push(uint64_t k)
{
    if (zs_cnt >= zs_lcap) {
        size_t nc = zs_lcap ? zs_lcap * 2 : 1024;
        uint64_t *t = (uint64_t *)realloc(zs_list, nc * sizeof(uint64_t));
        if (!t) abort();
        zs_list = t;
        zs_lcap = nc;
    }
    zs_list[zs_cnt++] = k;
}

seq_t a(idx_t n)
{
    if (!zs_init) {
        zs_init = 1;
        zs_mark(0);
        zs_push(0);
        zs_k = 0;
        zs_cur = 0;
    }
    while (zs_cnt < (size_t)n) {
        zs_k++;
        if (zs_k < zs_cap && zs_seen[zs_k]) {
            zs_cur = 0;
            zs_push(zs_k);
        } else {
            zs_cur += zs_k;
            zs_mark(zs_cur);
        }
    }
    return (seq_t)zs_list[(size_t)n - 1];
}
