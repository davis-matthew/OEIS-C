/*
 * A249918  Inverse permutation to A203069.
 * https://oeis.org/A249918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that A203069(k) = n; computed by generating
 *            A203069 (greedy search) until n appears
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9771 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

static uint64_t *A249918_pos = NULL;
static size_t A249918_cap = 0;
static uint64_t A249918_cnt = 0;
static uint64_t A249918_prev = 0;
static uint64_t A249918_lo[2] = {2, 1};

static void A249918_ensure(size_t idx)
{
    if (idx < A249918_cap) return;
    size_t nc = A249918_cap ? A249918_cap : 64;
    while (nc <= idx) nc <<= 1;
    uint64_t *p = (uint64_t *)realloc(A249918_pos, nc * sizeof(uint64_t));
    if (!p) abort();
    memset(p + A249918_cap, 0, (nc - A249918_cap) * sizeof(uint64_t));
    A249918_pos = p;
    A249918_cap = nc;
}

static void A249918_step(void)
{
    if (A249918_cnt == 0) {
        A249918_ensure(1);
        A249918_pos[1] = 1;
        A249918_cnt = 1;
        A249918_prev = 1;
        return;
    }
    int par = (int)((A249918_prev & 1) ^ 1);
    uint64_t c = A249918_lo[par];
    A249918_ensure((size_t)c);
    while (A249918_pos[c]) {
        c += 2;
        A249918_ensure((size_t)c);
    }
    A249918_lo[par] = c;
    while (true) {
        A249918_ensure((size_t)c);
        if (!A249918_pos[c] && !is_prime_u64(A249918_prev + c)) break;
        c += 2;
    }
    A249918_cnt++;
    A249918_pos[c] = A249918_cnt;
    A249918_prev = c;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    for (;;) {
        if ((size_t)n < A249918_cap && A249918_pos[n])
            return (seq_t)A249918_pos[n];
        A249918_step();
    }
}
