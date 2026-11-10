/*
 * A369051  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A003415(i) = A003415(j) and A085731(i) = A085731(j), for all i,
 *          j >= 1.
 * https://oeis.org/A369051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of [A003415(n),
 *            A085731(n)]: a(n) = order of first appearance of the pair (n',
 *            gcd(n,n')) among n=1..n (computed by incremental scan with
 *            hashing)
 * Range:     u8 n <= 380, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 107 terms through n = 9477 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint64_t d, g, v; int used; } A369051_slot;

static A369051_slot *A369051_tab = NULL;
static size_t A369051_cap = 0, A369051_cnt = 0;
static uint64_t *A369051_vals = NULL;
static uint64_t A369051_done = 0, A369051_vcap = 0;

static size_t A369051_hash(uint64_t d, uint64_t g) {
    uint64_t h = d * 0x9E3779B97F4A7C15ULL ^ (g + 0x7F4A7C15ULL) * 0xC2B2AE3D27D4EB4FULL;
    h ^= h >> 31;
    return (size_t)h;
}

static void A369051_grow(void) {
    size_t ncap = A369051_cap ? A369051_cap * 2 : 1024;
    A369051_slot *nt = (A369051_slot *)calloc(ncap, sizeof(A369051_slot));
    for (size_t i = 0; i < A369051_cap; i++) {
        if (A369051_tab[i].used) {
            size_t h = A369051_hash(A369051_tab[i].d, A369051_tab[i].g) & (ncap - 1);
            while (nt[h].used) h = (h + 1) & (ncap - 1);
            nt[h] = A369051_tab[i];
        }
    }
    free(A369051_tab);
    A369051_tab = nt;
    A369051_cap = ncap;
}

static uint64_t A369051_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t A369051_deriv(uint64_t n) {
    if (n <= 1) return 0;
    uint64_t res = 0, m = n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            res += (n / p) * e;
        }
    }
    if (m > 1) res += n / m;
    return res;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    if (N > A369051_vcap) {
        uint64_t nc = A369051_vcap ? A369051_vcap : 1024;
        while (nc < N) nc *= 2;
        A369051_vals = (uint64_t *)realloc(A369051_vals, (nc + 1) * sizeof(uint64_t));
        A369051_vcap = nc;
    }
    while (A369051_done < N) {
        uint64_t m = ++A369051_done;
        uint64_t d = A369051_deriv(m);
        uint64_t g = A369051_gcd(m, d);
        if ((A369051_cnt + 1) * 2 > A369051_cap) A369051_grow();
        size_t h = A369051_hash(d, g) & (A369051_cap - 1);
        while (A369051_tab[h].used && !(A369051_tab[h].d == d && A369051_tab[h].g == g))
            h = (h + 1) & (A369051_cap - 1);
        if (!A369051_tab[h].used) {
            A369051_tab[h].used = 1;
            A369051_tab[h].d = d;
            A369051_tab[h].g = g;
            A369051_tab[h].v = ++A369051_cnt;
        }
        A369051_vals[m] = A369051_tab[h].v;
    }
    return (seq_t)A369051_vals[N];
}
