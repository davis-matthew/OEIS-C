/*
 * A374040  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => f(i) = f(j), where f(n) = [A003415(n), A085731(n), A007814(n),
 *          A007949(n)], for all i, j >= 1.
 * https://oeis.org/A374040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of [A003415(n),
 *            A085731(n), A007814(n), A007949(n)]; computed incrementally with
 *            a hash of seen tuples
 * Range:     u8 n <= 332, u16 n <= 95307, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 114 terms through n = 8766 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *rg_keys = NULL;   /* 4 values per distinct id (id starting at 1) */
static uint32_t *rg_ids = NULL;    /* rg_ids[m] = a(m) */
static uint32_t *rg_tab = NULL;    /* hash table: id or 0 */
static size_t rg_tabsz = 0, rg_nd = 0, rg_done = 0, rg_cap = 0, rg_kcap = 0;

static uint64_t rg_hash(const uint64_t *k) {
    uint64_t h = 1469598103934665603ULL;
    for (int i = 0; i < 4; i++) {
        h ^= k[i];
        h *= 1099511628211ULL;
        h ^= h >> 29;
    }
    return h;
}

static void rg_rehash(size_t newsz) {
    free(rg_tab);
    rg_tab = (uint32_t *)calloc(newsz, sizeof(uint32_t));
    rg_tabsz = newsz;
    for (size_t id = 1; id <= rg_nd; id++) {
        size_t pos = (size_t)(rg_hash(&rg_keys[4 * id]) & (newsz - 1));
        while (rg_tab[pos]) pos = (pos + 1) & (newsz - 1);
        rg_tab[pos] = (uint32_t)id;
    }
}

static uint32_t rg_lookup(const uint64_t *k) {
    size_t pos = (size_t)(rg_hash(k) & (rg_tabsz - 1));
    while (rg_tab[pos]) {
        if (memcmp(&rg_keys[4 * (size_t)rg_tab[pos]], k, 4 * sizeof(uint64_t)) == 0)
            return rg_tab[pos];
        pos = (pos + 1) & (rg_tabsz - 1);
    }
    if ((rg_nd + 1) * 2 > rg_tabsz) {
        rg_rehash(rg_tabsz * 2);
        return rg_lookup(k);
    }
    rg_nd++;
    if ((rg_nd + 1) * 4 > rg_kcap) {
        rg_kcap = (rg_kcap ? rg_kcap * 2 : 4096);
        while ((rg_nd + 1) * 4 > rg_kcap) rg_kcap *= 2;
        rg_keys = (uint64_t *)realloc(rg_keys, rg_kcap * sizeof(uint64_t));
    }
    memcpy(&rg_keys[4 * rg_nd], k, 4 * sizeof(uint64_t));
    rg_tab[pos] = (uint32_t)rg_nd;
    return (uint32_t)rg_nd;
}

static void rg_tuple(uint64_t m, uint64_t *k) {
    uint64_t d = 0, r = m, v2 = 0, v3 = 0;
    if (m > 1) {
        for (uint64_t p = 2; p * p <= r; p += (p == 2 ? 1 : 2)) {
            if (r % p == 0) {
                uint64_t e = 0;
                while (r % p == 0) { r /= p; e++; }
                d += (m / p) * e;
                if (p == 2) v2 = e;
                if (p == 3) v3 = e;
            }
        }
        if (r > 1) {
            d += m / r;
            if (r == 2) v2 = 1;
            if (r == 3) v3 = 1;
        }
    }
    uint64_t a = m, b = d;
    while (b) { uint64_t t = a % b; a = b; b = t; }
    k[0] = d; k[1] = a; k[2] = v2; k[3] = v3;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (!rg_tab) rg_rehash(1024);
    if ((size_t)n >= rg_cap) {
        size_t nc = rg_cap ? rg_cap : 1024;
        while (nc <= (size_t)n) nc *= 2;
        rg_ids = (uint32_t *)realloc(rg_ids, nc * sizeof(uint32_t));
        rg_cap = nc;
    }
    while (rg_done < (size_t)n) {
        uint64_t k[4];
        rg_tuple((uint64_t)(rg_done + 1), k);
        rg_ids[rg_done + 1] = rg_lookup(k);
        rg_done++;
    }
    return (seq_t)rg_ids[(size_t)n];
}
