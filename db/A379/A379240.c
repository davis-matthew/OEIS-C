/*
 * A379240  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => f(i) = f(j), for all i, j, where f(n) = [A003415(n),
 *          A085731(n)] if A359550(n) = 1, otherwise f(n) = n.
 * https://oeis.org/A379240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of f(n), where f(n) =
 *            [A003415(n), A085731(n)] if A359550(n) = 1, else f(n) = n;
 *            computed by a first-occurrence scan with a hash table
 * Range:     u8 n <= 367, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 108 terms through n = 9716 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static uint64_t *A379240_id = NULL;
static uint64_t *A379240_d = NULL;
static uint64_t *A379240_g = NULL;
static unsigned char *A379240_first = NULL;
static uint64_t *A379240_tab = NULL;
static uint64_t A379240_tcap = 0;
static uint64_t A379240_done = 0;
static uint64_t A379240_cap = 0;
static uint64_t A379240_cnt = 0;

static uint64_t A379240_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t A379240_hash(uint64_t d, uint64_t g) {
    uint64_t h = d * 0x9E3779B97F4A7C15ULL ^ (g + 0x7F4A7C15ULL) * 0xC2B2AE3D27D4EB4FULL;
    h ^= h >> 29;
    return h;
}

static void A379240_insert(uint64_t m) {
    uint64_t mask = A379240_tcap - 1;
    uint64_t h = A379240_hash(A379240_d[m], A379240_g[m]) & mask;
    while (A379240_tab[h]) h = (h + 1) & mask;
    A379240_tab[h] = m;
}

static void A379240_extend(uint64_t n) {
    if (n <= A379240_done) return;
    if (n + 1 > A379240_cap) {
        uint64_t nc = A379240_cap ? A379240_cap : 1024;
        while (nc < n + 1) nc *= 2;
        A379240_id = (uint64_t *)realloc(A379240_id, nc * sizeof(uint64_t));
        A379240_d = (uint64_t *)realloc(A379240_d, nc * sizeof(uint64_t));
        A379240_g = (uint64_t *)realloc(A379240_g, nc * sizeof(uint64_t));
        A379240_first = (unsigned char *)realloc(A379240_first, nc);
        A379240_cap = nc;
        /* rebuild hash table */
        uint64_t tc = 2048;
        while (tc < nc * 2) tc *= 2;
        free(A379240_tab);
        A379240_tab = (uint64_t *)calloc(tc, sizeof(uint64_t));
        A379240_tcap = tc;
        for (uint64_t m = 1; m <= A379240_done; m++)
            if (A379240_first[m]) A379240_insert(m);
    }
    for (uint64_t m = A379240_done + 1; m <= n; m++) {
        uint64_t r = m, der = 0;
        bool ok = true;
        for (uint64_t p = 2; p * p <= r; p++) {
            if (r % p == 0) {
                uint64_t e = 0;
                while (r % p == 0) { r /= p; e++; }
                if (p <= e) ok = false;
                der += e * (m / p);
            }
        }
        if (r > 1) der += m / r;
        A379240_first[m] = 0;
        if (!ok) {
            A379240_id[m] = ++A379240_cnt;
            A379240_d[m] = 0; A379240_g[m] = 0;
        } else {
            uint64_t g = A379240_gcd(m, der);
            A379240_d[m] = der; A379240_g[m] = g;
            uint64_t mask = A379240_tcap - 1;
            uint64_t h = A379240_hash(der, g) & mask;
            uint64_t found = 0;
            while (A379240_tab[h]) {
                uint64_t q = A379240_tab[h];
                if (A379240_d[q] == der && A379240_g[q] == g) { found = q; break; }
                h = (h + 1) & mask;
            }
            if (found) {
                A379240_id[m] = A379240_id[found];
            } else {
                A379240_id[m] = ++A379240_cnt;
                A379240_first[m] = 1;
                A379240_tab[h] = m;
            }
        }
    }
    A379240_done = n;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    A379240_extend((uint64_t)n);
    return (seq_t)A379240_id[n];
}
