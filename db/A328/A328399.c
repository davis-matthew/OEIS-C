/*
 * A328399  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A328475(i) = A328475(j) for all i, j.
 * https://oeis.org/A328399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of A328475, where
 *            A328475(n) = A111701(A276086(n)); computed incrementally by
 *            classifying primorial-base digit vectors via a hash table
 * Range:     u8 n <= 509, u16 n <= 32768, u32 n <= 32768, u64 n <= 32768,
 *            u128 n <= 32768, big any n
 * Validated: standard, pass, 112 terms through n = 9311 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

typedef struct { unsigned char d[16]; } A328399_key;

static const unsigned A328399_P[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};

static A328399_key *A328399_keys = NULL;
static size_t A328399_nkeys = 0, A328399_keycap = 0;
static size_t *A328399_tab = NULL;
static size_t A328399_tabcap = 0;
static uint64_t *A328399_cls = NULL;
static size_t A328399_ncls = 0, A328399_clscap = 0;

static size_t A328399_hash(const A328399_key *k)
{
    uint64_t h = 1469598103934665603ULL;
    for (int i = 0; i < 16; ++i) {
        h ^= k->d[i];
        h *= 1099511628211ULL;
    }
    h ^= h >> 29;
    return (size_t)h;
}

static void A328399_rehash(size_t newcap)
{
    size_t *t = (size_t *)calloc(newcap, sizeof(size_t));
    if (!t) abort();
    for (size_t i = 0; i < A328399_nkeys; ++i) {
        size_t h = A328399_hash(&A328399_keys[i]) & (newcap - 1);
        while (t[h]) h = (h + 1) & (newcap - 1);
        t[h] = i + 1;
    }
    free(A328399_tab);
    A328399_tab = t;
    A328399_tabcap = newcap;
}

static uint64_t A328399_lookup(const A328399_key *k)
{
    if (A328399_tabcap == 0) A328399_rehash(1024);
    size_t h = A328399_hash(k) & (A328399_tabcap - 1);
    while (A328399_tab[h]) {
        size_t id = A328399_tab[h];
        if (memcmp(&A328399_keys[id - 1], k, sizeof(A328399_key)) == 0)
            return (uint64_t)id;
        h = (h + 1) & (A328399_tabcap - 1);
    }
    if (A328399_nkeys == A328399_keycap) {
        A328399_keycap = A328399_keycap ? A328399_keycap * 2 : 1024;
        A328399_keys = (A328399_key *)realloc(A328399_keys, A328399_keycap * sizeof(A328399_key));
        if (!A328399_keys) abort();
    }
    A328399_keys[A328399_nkeys++] = *k;
    A328399_tab[h] = A328399_nkeys;
    uint64_t res = (uint64_t)A328399_nkeys;
    if (A328399_nkeys * 2 > A328399_tabcap) A328399_rehash(A328399_tabcap * 2);
    return res;
}

static uint64_t A328399_class_of(uint64_t m)
{
    A328399_key k;
    memset(&k, 0, sizeof(k));
    uint64_t rem = m;
    int len = 0;
    while (rem > 0 && len < 16) {
        k.d[len] = (unsigned char)(rem % A328399_P[len]);
        rem /= A328399_P[len];
        ++len;
    }
    for (int i = 0; i < len; ++i) {
        if (k.d[i] > 0) k.d[i] -= 1;
        else break;
    }
    return A328399_lookup(&k);
}

seq_t a(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need > A328399_clscap) {
        size_t nc = A328399_clscap ? A328399_clscap : 1024;
        while (nc < need) nc *= 2;
        A328399_cls = (uint64_t *)realloc(A328399_cls, nc * sizeof(uint64_t));
        if (!A328399_cls) abort();
        A328399_clscap = nc;
    }
    while (A328399_ncls < need) {
        A328399_cls[A328399_ncls] = A328399_class_of((uint64_t)A328399_ncls);
        ++A328399_ncls;
    }
    return (seq_t)A328399_cls[(size_t)n];
}
