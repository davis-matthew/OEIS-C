/*
 * A375756  Inverse permutation to A375755.
 * https://oeis.org/A375756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k such that A375755(k) = n; A375755 is the greedy
 *            sequence of distinct positive integers whose consecutive terms
 *            have different leading digits. The code generates A375755 until
 *            n appears and records its position
 * Range:     u8 n <= 182, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9665 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t *A375756_pos = NULL;   /* pos[v] = index k with A375755(k)=v, 0 if not yet */
static size_t A375756_cap = 0;
static uint64_t A375756_ptr[10];
static int A375756_prev = 0;
static uint64_t A375756_count = 0;
static bool A375756_init = false;

static void A375756_ensure(size_t needed)
{
    if (needed < A375756_cap) return;
    size_t nc = A375756_cap ? A375756_cap : 1024;
    while (nc <= needed) nc <<= 1;
    uint64_t *t = (uint64_t *)realloc(A375756_pos, nc * sizeof(uint64_t));
    if (!t) exit(1);
    for (size_t i = A375756_cap; i < nc; ++i) t[i] = 0;
    A375756_pos = t;
    A375756_cap = nc;
}

static void A375756_step(void)
{
    uint64_t chosen = (uint64_t)-1;
    int cd = 0;
    for (int d = 1; d <= 9; ++d) {
        if (d == A375756_prev) continue;
        if (A375756_ptr[d] < chosen) { chosen = A375756_ptr[d]; cd = d; }
    }
    A375756_count++;
    A375756_ensure((size_t)chosen);
    A375756_pos[chosen] = A375756_count;
    A375756_prev = cd;

    uint64_t next = chosen + 1;
    uint64_t p = 1;
    while (next / p >= 10) p *= 10;
    uint64_t lead = next / p;
    if (lead != (uint64_t)cd) {
        if (lead > (uint64_t)cd) next = p * 10 * (uint64_t)cd;
        else next = p * (uint64_t)cd;
    }
    A375756_ptr[cd] = next;
}

seq_t a(idx_t n)
{
    if (!A375756_init) {
        for (int d = 1; d <= 9; ++d) A375756_ptr[d] = (uint64_t)d;
        A375756_ensure(1024);
        A375756_init = true;
    }
    if (n == 0) return 0;
    A375756_ensure((size_t)n);
    while (A375756_pos[(size_t)n] == 0) {
        A375756_step();
        if ((size_t)n >= A375756_cap) A375756_ensure((size_t)n);
    }
    return (seq_t)A375756_pos[(size_t)n];
}
