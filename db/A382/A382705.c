/*
 * A382705  Numbers k which are "delayed" in A064413, that is, k appears after
 *          the k-th term.
 * https://oeis.org/A382705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A064413(A382704(n)): the EKG values b(m) at indices m
 *            with b(m) < m, found by generating the EKG sequence
 * Range:     u8 n <= 44, u16 n <= 699, u32 n <= 699, u64 n <= 699,
 *            u128 n <= 699, big any n
 * Validated: standard, pass, 89 terms through n = 677 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint8_t  *A382705_used = NULL;
static uint32_t *A382705_nxt  = NULL;
static size_t    A382705_cap  = 0;
static uint64_t *A382705_res  = NULL;
static size_t    A382705_rn   = 0, A382705_rcap = 0;
static uint64_t  A382705_prev = 0, A382705_idx = 0;
static bool      A382705_init = false;

static void A382705_grow(size_t need)
{
    if (need < A382705_cap) return;
    size_t nc = A382705_cap ? A382705_cap : 4096;
    while (nc <= need) nc <<= 1;
    uint8_t *u = (uint8_t *)realloc(A382705_used, nc);
    uint32_t *x = (uint32_t *)realloc(A382705_nxt, nc * sizeof(uint32_t));
    if (!u || !x) exit(1);
    for (size_t i = A382705_cap; i < nc; ++i) { u[i] = 0; x[i] = 0; }
    A382705_used = u;
    A382705_nxt = x;
    A382705_cap = nc;
}

static void A382705_step(void)
{
    uint64_t prev = A382705_prev;
    uint64_t primes[20];
    int pc = 0;
    uint64_t x = prev;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            primes[pc++] = p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) primes[pc++] = x;

    uint64_t best = 0;
    for (int i = 0; i < pc; ++i) {
        uint64_t p = primes[i];
        A382705_grow((size_t)p);
        uint64_t y = A382705_nxt[p] ? A382705_nxt[p] : p;
        for (;;) {
            if (y >= A382705_cap) A382705_grow((size_t)y);
            if (!A382705_used[y]) break;
            y += p;
        }
        A382705_nxt[p] = (uint32_t)y;
        if (best == 0 || y < best) best = y;
    }
    A382705_grow((size_t)best);
    A382705_used[best] = 1;
    ++A382705_idx;
    if (best < A382705_idx) {
        if (A382705_rn == A382705_rcap) {
            A382705_rcap = A382705_rcap ? A382705_rcap * 2 : 64;
            A382705_res = (uint64_t *)realloc(A382705_res, A382705_rcap * sizeof(uint64_t));
            if (!A382705_res) exit(1);
        }
        A382705_res[A382705_rn++] = best;
    }
    A382705_prev = best;
}

seq_t a(idx_t n)
{
    if (!A382705_init) {
        A382705_grow(4096);
        A382705_used[1] = 1;
        A382705_used[2] = 1;
        A382705_prev = 2;
        A382705_idx = 2;
        A382705_init = true;
    }
    if (n == 0) return (seq_t)0;
    while (A382705_rn < (size_t)n) A382705_step();
    return (seq_t)A382705_res[(size_t)n - 1];
}
