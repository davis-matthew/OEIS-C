/*
 * A366474  Where n appears in A337136, or -1 if n never appears.
 * https://oeis.org/A366474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that A337136(k) = n (A337136 is a permutation, so
 *            never -1); computed by generating A337136 until n appears.
 *            A337136: a(1)=1, a(2)=2, a(n)=smallest unused number sharing a
 *            factor with a(n-2)+a(n-1)
 * Range:     u8 n <= 150, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9281 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

static uint64_t *A366474_pos = NULL;  /* pos[v] = index of v in A337136, 0 if unseen */
static uint64_t *A366474_ptr = NULL;  /* ptr[p] = candidate multiple of p to resume from */
static size_t A366474_cap = 0;
static uint64_t A366474_len = 0, A366474_p1 = 0, A366474_p2 = 0;

static void A366474_ensure(uint64_t needed)
{
    if (needed < A366474_cap)
        return;
    size_t nc = A366474_cap ? A366474_cap : 1024;
    while (nc <= needed)
        nc <<= 1;
    uint64_t *a = (uint64_t *)realloc(A366474_pos, nc * sizeof(uint64_t));
    if (!a) abort();
    A366474_pos = a;
    uint64_t *b = (uint64_t *)realloc(A366474_ptr, nc * sizeof(uint64_t));
    if (!b) abort();
    A366474_ptr = b;
    memset(A366474_pos + A366474_cap, 0, (nc - A366474_cap) * sizeof(uint64_t));
    memset(A366474_ptr + A366474_cap, 0, (nc - A366474_cap) * sizeof(uint64_t));
    A366474_cap = nc;
}

static uint64_t A366474_next_multiple(uint64_t p)
{
    A366474_ensure(p);
    uint64_t c = A366474_ptr[p] ? A366474_ptr[p] : p;
    for (;;) {
        A366474_ensure(c);
        if (!A366474_pos[c])
            break;
        c += p;
    }
    A366474_ensure(p);
    A366474_ptr[p] = c;
    return c;
}

static void A366474_step(void)
{
    uint64_t s = A366474_p1 + A366474_p2;
    uint64_t best = UINT64_MAX;
    uint64_t m = s;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            uint64_t c = A366474_next_multiple(p);
            if (c < best) best = c;
        }
    }
    if (m > 1) {
        uint64_t c = A366474_next_multiple(m);
        if (c < best) best = c;
    }
    A366474_len++;
    A366474_ensure(best);
    A366474_pos[best] = A366474_len;
    A366474_p1 = A366474_p2;
    A366474_p2 = best;
}

seq_t a(idx_t n)
{
    if (A366474_len == 0) {
        A366474_ensure(2);
        A366474_pos[1] = 1;
        A366474_pos[2] = 2;
        A366474_len = 2;
        A366474_p1 = 1;
        A366474_p2 = 2;
    }
    uint64_t v = (uint64_t)n;
    for (;;) {
        if (v < A366474_cap && A366474_pos[v])
            return (seq_t)A366474_pos[v];
        A366474_step();
    }
}
