/*
 * A323370  Lexicographically earliest sequence such that for all i, j, a(i) =
 *          a(j) => f(i) = f(j) where f(n) = [A000035(n), A003557(n),
 *          A023900(n)] for all other numbers, except f(n) = 0 for odd primes.
 * https://oeis.org/A323370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   restricted growth sequence transform of f(n) = 0 for odd primes,
 *            else [n mod 2, n/rad(n), prod_{p|n}(1-p)]; computed by scanning
 *            1..n and numbering classes by first appearance
 * Range:     u8 n <= 345, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9912 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    uint64_t q, ph, tag, id;
    bool used;
} a323370_ent;

static a323370_ent *a323370_tab = NULL;
static size_t a323370_cap = 0, a323370_cnt = 0;
static uint64_t *a323370_ids = NULL;
static uint64_t a323370_done = 0, a323370_idcap = 0;

static size_t a323370_hash(uint64_t q, uint64_t ph, uint64_t tag) {
    uint64_t h = q * 0x9E3779B97F4A7C15ULL;
    h ^= (h >> 29);
    h += ph * 0xC2B2AE3D27D4EB4FULL;
    h ^= (h >> 31);
    h += tag * 0x165667B19E3779F9ULL;
    h ^= (h >> 27);
    h *= 0x94D049BB133111EBULL;
    h ^= (h >> 32);
    return (size_t)h;
}

static void a323370_insert_raw(const a323370_ent *e) {
    size_t i = a323370_hash(e->q, e->ph, e->tag) & (a323370_cap - 1);
    while (a323370_tab[i].used) i = (i + 1) & (a323370_cap - 1);
    a323370_tab[i] = *e;
}

static void a323370_grow(void) {
    size_t oldcap = a323370_cap;
    a323370_ent *old = a323370_tab;
    a323370_cap = oldcap ? oldcap * 2 : 1024;
    a323370_tab = (a323370_ent *)calloc(a323370_cap, sizeof(a323370_ent));
    for (size_t i = 0; i < oldcap; i++)
        if (old[i].used) a323370_insert_raw(&old[i]);
    free(old);
}

static uint64_t a323370_lookup(uint64_t q, uint64_t ph, uint64_t tag) {
    if ((a323370_cnt + 1) * 2 > a323370_cap) a323370_grow();
    size_t i = a323370_hash(q, ph, tag) & (a323370_cap - 1);
    while (a323370_tab[i].used) {
        if (a323370_tab[i].q == q && a323370_tab[i].ph == ph && a323370_tab[i].tag == tag)
            return a323370_tab[i].id;
        i = (i + 1) & (a323370_cap - 1);
    }
    a323370_ent e;
    e.q = q; e.ph = ph; e.tag = tag; e.id = (uint64_t)a323370_cnt + 1; e.used = true;
    a323370_tab[i] = e;
    a323370_cnt++;
    return e.id;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    if (N > a323370_done) {
        if (N + 1 > a323370_idcap) {
            uint64_t nc = a323370_idcap ? a323370_idcap : 1024;
            while (nc < N + 1) nc *= 2;
            a323370_ids = (uint64_t *)realloc(a323370_ids, nc * sizeof(uint64_t));
            a323370_idcap = nc;
        }
        for (uint64_t k = a323370_done + 1; k <= N; k++) {
            uint64_t m = k, rad = 1, ph = 1, om = 0;
            for (uint64_t p = 2; p * p <= m; p++) {
                if (m % p == 0) {
                    while (m % p == 0) m /= p;
                    rad *= p; ph *= (p - 1); om++;
                }
            }
            if (m > 1) { rad *= m; ph *= (m - 1); om++; }
            uint64_t q = k / rad;
            uint64_t id;
            if ((k & 1) && om == 1 && q == 1)
                id = a323370_lookup(0, 0, 0);
            else
                id = a323370_lookup(q, ph, (k & 1) | ((om & 1) << 1));
            a323370_ids[k] = id;
        }
        a323370_done = N;
    }
    return (seq_t)a323370_ids[N];
}
