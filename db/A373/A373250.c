/*
 * A373250  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A181819(i) = A181819(j) and i mod A181819(i) = j mod
 *          A181819(j), for all i, j >= 1, where A181819 is the prime shadow
 *          of n.
 * https://oeis.org/A373250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pairs
 *            [A181819(n), n mod A181819(n)]; computed by a memoized scan
 *            assigning ids in order of first appearance
 * Range:     u8 n <= 3135, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 115 terms through n = 9048 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static uint64_t A373250_nth_prime(unsigned k)
{
    unsigned cnt = 0;
    uint64_t c = 1;
    while (cnt < k) {
        c++;
        bool ok = true;
        for (uint64_t d = 2; d * d <= c; d++)
            if (c % d == 0) { ok = false; break; }
        if (ok) cnt++;
    }
    return c;
}

static uint64_t A373250_shadow(uint64_t n)
{
    static uint64_t pr[80];
    static bool init = false;
    if (!init) {
        for (unsigned i = 0; i < 80; i++) pr[i] = A373250_nth_prime(i + 1);
        init = true;
    }
    uint64_t res = 1;
    for (uint64_t p = 2; p * p <= n; p += (p == 2 ? 1 : 2)) {
        if (n % p == 0) {
            unsigned e = 0;
            while (n % p == 0) { n /= p; e++; }
            res *= pr[e - 1];
        }
    }
    if (n > 1) res *= pr[0];
    return res;
}

typedef struct { uint64_t s, r; uint32_t id; } A373250_ent;

static A373250_ent *A373250_tab = NULL;
static size_t A373250_cap = 0, A373250_used = 0;
static uint32_t *A373250_ids = NULL;
static size_t A373250_idcap = 0;
static uint64_t A373250_done = 0;

static size_t A373250_hash(uint64_t s, uint64_t r, size_t mask)
{
    uint64_t h = s * 0x9E3779B97F4A7C15ULL ^ (r + 0x7F4A7C15ULL) * 0xC2B2AE3D27D4EB4FULL;
    h ^= h >> 29;
    return (size_t)h & mask;
}

static void A373250_grow(void)
{
    size_t ncap = A373250_cap ? A373250_cap * 2 : 1024;
    A373250_ent *nt = (A373250_ent *)calloc(ncap, sizeof(A373250_ent));
    if (!nt) abort();
    for (size_t i = 0; i < A373250_cap; i++) {
        if (A373250_tab[i].id) {
            size_t h = A373250_hash(A373250_tab[i].s, A373250_tab[i].r, ncap - 1);
            while (nt[h].id) h = (h + 1) & (ncap - 1);
            nt[h] = A373250_tab[i];
        }
    }
    free(A373250_tab);
    A373250_tab = nt;
    A373250_cap = ncap;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n > A373250_idcap) {
        size_t nc = A373250_idcap ? A373250_idcap : 1024;
        while (nc < (size_t)n) nc *= 2;
        A373250_ids = (uint32_t *)realloc(A373250_ids, (nc + 1) * sizeof(uint32_t));
        if (!A373250_ids) abort();
        A373250_idcap = nc;
    }
    while (A373250_done < n) {
        uint64_t m = A373250_done + 1;
        uint64_t s = A373250_shadow(m);
        uint64_t r = m % s;
        if ((A373250_used + 1) * 2 > A373250_cap) A373250_grow();
        size_t h = A373250_hash(s, r, A373250_cap - 1);
        uint32_t id = 0;
        while (A373250_tab[h].id) {
            if (A373250_tab[h].s == s && A373250_tab[h].r == r) { id = A373250_tab[h].id; break; }
            h = (h + 1) & (A373250_cap - 1);
        }
        if (!id) {
            A373250_used++;
            id = (uint32_t)A373250_used;
            A373250_tab[h].s = s;
            A373250_tab[h].r = r;
            A373250_tab[h].id = id;
        }
        A373250_ids[m] = id;
        A373250_done = m;
    }
    return (seq_t)A373250_ids[n];
}
