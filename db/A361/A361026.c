/*
 * A361026  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A003557(i) = A003557(j) and A053669(i) = A053669(j), for all i,
 *          j >= 1.
 * https://oeis.org/A361026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of [A003557(n),
 *            A053669(n)]; computed by scanning k=1..n and numbering distinct
 *            pairs in order of first appearance
 * Range:     u8 n <= 5399, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9708 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *rg_g = NULL, *rg_p = NULL; /* distinct pairs, index = rank-1 */
static uint64_t *rg_rank = NULL;            /* rank[k] for k = 1..rg_done */
static uint64_t rg_done = 0, rg_cap = 0, rg_dcount = 0;

static uint64_t A003557_core(uint64_t n) {
    uint64_t result = 1, temp = n;
    for (uint64_t p = 2; p * p <= temp; p++) {
        uint64_t e = 0;
        while (temp % p == 0) { temp /= p; e++; }
        for (uint64_t i = 1; i < e; i++) result *= p;
    }
    return result;
}

static uint64_t A053669_core(uint64_t n) {
    uint64_t p = 2;
    for (;;) {
        int prime = 1;
        for (uint64_t d = 2; d * d <= p; d++)
            if (p % d == 0) { prime = 0; break; }
        if (prime && n % p != 0) return p;
        p++;
    }
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    if (N > rg_done) {
        if (N + 1 > rg_cap) {
            uint64_t nc = (N + 1) * 2;
            rg_g = (uint64_t *)realloc(rg_g, nc * sizeof(uint64_t));
            rg_p = (uint64_t *)realloc(rg_p, nc * sizeof(uint64_t));
            rg_rank = (uint64_t *)realloc(rg_rank, nc * sizeof(uint64_t));
            rg_cap = nc;
        }
        for (uint64_t k = rg_done + 1; k <= N; k++) {
            uint64_t g = A003557_core(k), p = A053669_core(k);
            uint64_t found = 0;
            for (uint64_t i = 0; i < rg_dcount; i++) {
                if (rg_g[i] == g && rg_p[i] == p) { found = i + 1; break; }
            }
            if (!found) {
                rg_g[rg_dcount] = g;
                rg_p[rg_dcount] = p;
                rg_dcount++;
                found = rg_dcount;
            }
            rg_rank[k] = found;
        }
        rg_done = N;
    }
    return (seq_t)rg_rank[N];
}
