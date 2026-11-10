/*
 * A334868  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => f(i) = f(j) for all i, j >= 1, where f(1) = 0 and for n > 1,
 *          f(n) = -1 if n is in A050376, and f(n) = A334870(n) otherwise.
 * https://oeis.org/A334868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank of first occurrence of f(n) among f(1), f(2), ...
 *            (restricted growth labeling), f(1)=0, f(n)=-1 if n in A050376,
 *            else f(n)=A334870(n); computed by hashing prime-exponent vectors
 *            of f(n)
 * Range:     u8 n <= 338, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9799 (2026-11-08)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

typedef struct { uint64_t h1, h2; uint32_t id; } A334868_ent;

static A334868_ent *A334868_tab = NULL;
static size_t A334868_cap = 0;
static uint32_t *A334868_val = NULL;
static size_t A334868_cnt = 0, A334868_vcap = 0;
static uint32_t A334868_classes = 0;

static uint64_t A334868_mix(uint64_t x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

static void A334868_insert_raw(uint64_t h1, uint64_t h2, uint32_t id) {
    size_t m = A334868_cap - 1;
    size_t i = (size_t)(h1 ^ (h2 >> 7)) & m;
    while (A334868_tab[i].id) i = (i + 1) & m;
    A334868_tab[i].h1 = h1; A334868_tab[i].h2 = h2; A334868_tab[i].id = id;
}

static void A334868_grow(void) {
    size_t oldcap = A334868_cap;
    A334868_ent *old = A334868_tab;
    A334868_cap = oldcap ? oldcap * 2 : 1024;
    A334868_tab = (A334868_ent *)calloc(A334868_cap, sizeof(A334868_ent));
    for (size_t i = 0; i < oldcap; i++)
        if (old[i].id) A334868_insert_raw(old[i].h1, old[i].h2, old[i].id);
    free(old);
}

static uint32_t A334868_classify(uint64_t h1, uint64_t h2) {
    if ((A334868_classes + 1) * 2 > A334868_cap) A334868_grow();
    size_t m = A334868_cap - 1;
    size_t i = (size_t)(h1 ^ (h2 >> 7)) & m;
    while (A334868_tab[i].id) {
        if (A334868_tab[i].h1 == h1 && A334868_tab[i].h2 == h2) return A334868_tab[i].id;
        i = (i + 1) & m;
    }
    A334868_classes++;
    A334868_tab[i].h1 = h1; A334868_tab[i].h2 = h2; A334868_tab[i].id = A334868_classes;
    return A334868_classes;
}

static void A334868_key(uint64_t n, uint64_t *o1, uint64_t *o2) {
    if (n == 1) { *o1 = 1; *o2 = 1; return; }
    uint64_t qs[20]; int es[20]; int c = 0;
    uint64_t m = n;
    for (uint64_t d = 2; d * d <= m; d++) {
        if (m % d == 0) {
            int e = 0;
            while (m % d == 0) { m /= d; e++; }
            qs[c] = d; es[c] = e; c++;
        }
    }
    if (m > 1) { qs[c] = m; es[c] = 1; c++; }
    if (c == 1 && (es[0] & (es[0] - 1)) == 0) { *o1 = 2; *o2 = 2; return; }
    bool sq = true;
    for (int i = 0; i < c; i++) if (es[i] & 1) sq = false;
    uint64_t h1 = 0, h2 = 0;
    if (sq) {
        for (int i = 0; i < c; i++) {
            h1 += (uint64_t)(es[i] / 2) * A334868_mix(qs[i] * 2 + 11);
            h2 += (uint64_t)(es[i] / 2) * A334868_mix(qs[i] * 4 + 977);
        }
    } else {
        uint64_t p = 0;
        for (int i = 0; i < c; i++) if (es[i] & 1) { p = qs[i]; break; }
        for (int i = 0; i < c; i++) {
            uint64_t e = (uint64_t)es[i];
            if (qs[i] == p) e--;
            h1 += e * A334868_mix(qs[i] * 2 + 11);
            h2 += e * A334868_mix(qs[i] * 4 + 977);
        }
        for (uint64_t q = 2; q < p; q++) {
            if (is_prime((idx_t)q)) {
                h1 += A334868_mix(q * 2 + 11);
                h2 += A334868_mix(q * 4 + 977);
            }
        }
    }
    *o1 = h1; *o2 = h2;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    size_t need = (size_t)n;
    if (need > A334868_vcap) {
        size_t nc = A334868_vcap ? A334868_vcap : 1024;
        while (nc < need) nc *= 2;
        A334868_val = (uint32_t *)realloc(A334868_val, nc * sizeof(uint32_t));
        A334868_vcap = nc;
    }
    while (A334868_cnt < need) {
        uint64_t k = (uint64_t)A334868_cnt + 1, h1, h2;
        A334868_key(k, &h1, &h2);
        A334868_val[A334868_cnt] = A334868_classify(h1, h2);
        A334868_cnt++;
    }
    return (seq_t)A334868_val[need - 1];
}
