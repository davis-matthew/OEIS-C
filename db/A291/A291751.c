/*
 * A291751  Lexicographically earliest such sequence a that a(i) = a(j) =>
 *          A003557(i) = A003557(j) and A048250(i) = A048250(j), for all i, j.
 * https://oeis.org/A291751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   restricted growth sequence transform of the pair (A003557(n),
 *            A048250(n)); a(n) = 1 + number of distinct pairs first seen
 *            before the first occurrence of n's pair. Computed by incremental
 *            scan with a hash table
 * Range:     u8 n <= 365, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 109 terms through n = 9976 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint64_t x, y; uint32_t id; } A291751_ent;

#define A291751_HB 20
#define A291751_HS ((size_t)1 << A291751_HB)

static A291751_ent *A291751_tab = NULL;
static uint32_t *A291751_cls = NULL;
static size_t A291751_cap = 0;
static uint64_t A291751_done = 0;
static uint32_t A291751_next = 0;

static void A291751_pair(uint64_t n, uint64_t *x, uint64_t *y) {
    uint64_t m = n, rx = 1, ry = 1;
    for (uint64_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            for (int i = 1; i < e; i++) rx *= p;
            ry *= (p + 1);
        }
    }
    if (m > 1) ry *= (m + 1);
    *x = rx; *y = ry;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    if (!A291751_tab) {
        A291751_tab = (A291751_ent *)calloc(A291751_HS, sizeof(A291751_ent));
    }
    if (N > A291751_cap) {
        size_t nc = A291751_cap ? A291751_cap : 1024;
        while (nc < N) nc *= 2;
        A291751_cls = (uint32_t *)realloc(A291751_cls, (nc + 1) * sizeof(uint32_t));
        A291751_cap = nc;
    }
    while (A291751_done < N) {
        uint64_t k = ++A291751_done, x, y;
        A291751_pair(k, &x, &y);
        uint64_t h = (x * 0x9E3779B97F4A7C15ULL) ^ (y * 0xC2B2AE3D27D4EB4FULL);
        h ^= h >> 29;
        size_t i = (size_t)(h & (A291751_HS - 1));
        for (;;) {
            A291751_ent *e = &A291751_tab[i];
            if (e->id == 0) {
                e->x = x; e->y = y; e->id = ++A291751_next;
                A291751_cls[k] = e->id;
                break;
            }
            if (e->x == x && e->y == y) { A291751_cls[k] = e->id; break; }
            i = (i + 1) & (A291751_HS - 1);
        }
    }
    return (seq_t)A291751_cls[N];
}
