/*
 * A369171  Numbers k such that A000688(k) = A046660(k+1).
 * https://oeis.org/A369171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k such that A000688(k) = A046660(k+1); found by a
 *            incremental scan, with terms cached
 * Range:     u8 n <= 37, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 7998 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A369171_part[64];
static int A369171_part_init = 0;

static void A369171_init(void) {
    if (A369171_part_init) return;
    for (int i = 0; i < 64; i++) A369171_part[i] = 0;
    A369171_part[0] = 1;
    for (int p = 1; p < 64; p++)
        for (int i = p; i < 64; i++) A369171_part[i] += A369171_part[i - p];
    A369171_part_init = 1;
}

/* Returns number of abelian groups of order m and the excess Omega-omega. */
static void A369171_info(uint64_t m, uint64_t *groups, uint64_t *excess) {
    uint64_t g = 1, ex = 0;
    uint64_t p = 2;
    while (p * p <= m) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            g *= A369171_part[e];
            ex += (uint64_t)(e - 1);
        }
        p += (p == 2) ? 1 : 2;
    }
    *groups = g;
    *excess = ex;
}

static idx_t *A369171_terms = NULL;
static size_t A369171_cnt = 0, A369171_cap = 0;
static idx_t A369171_next = 1;

seq_t a(idx_t n) {
    A369171_init();
    while (A369171_cnt < (size_t)n) {
        uint64_t k = (uint64_t)A369171_next;
        uint64_t g, e, g2, e2;
        A369171_info(k, &g, &e);
        A369171_info(k + 1, &g2, &e2);
        if (g == e2) {
            if (A369171_cnt == A369171_cap) {
                size_t nc = A369171_cap ? A369171_cap * 2 : 1024;
                idx_t *nt = (idx_t *)realloc(A369171_terms, nc * sizeof(idx_t));
                if (!nt) return (seq_t)0;
                A369171_terms = nt;
                A369171_cap = nc;
            }
            A369171_terms[A369171_cnt++] = (idx_t)k;
        }
        A369171_next++;
    }
    return (seq_t)A369171_terms[n - 1];
}
