/*
 * A371249  Inverse permutation to A371248.
 * https://oeis.org/A371249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that A371248(k) = n (inverse permutation);
 *            computed by running the greedy construction of A371248 until n
 *            appears
 * Range:     u8 n <= 240, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9896 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t *A371249_pos = NULL;   /* pos[v] = index of value v, 0 if unused */
static size_t A371249_cap = 0;

static seq_t *A371249_next = NULL;     /* next candidate per column, 0 = uninit */
static size_t A371249_ncap = 0;

static uint64_t A371249_count = 0;
static size_t A371249_pos_in_diag = 0;
static size_t A371249_diag_sum = 2;
static size_t A371249_smallest = 1;

static void A371249_grow(size_t v)
{
    if (v < A371249_cap)
        return;
    size_t nc = A371249_cap ? A371249_cap : 64;
    while (nc <= v)
        nc <<= 1;
    uint64_t *t = (uint64_t *)realloc(A371249_pos, nc * sizeof(uint64_t));
    if (!t)
        abort();
    for (size_t i = A371249_cap; i < nc; ++i)
        t[i] = 0;
    A371249_pos = t;
    A371249_cap = nc;
}

static void A371249_grow_cols(size_t c)
{
    if (c < A371249_ncap)
        return;
    size_t nc = A371249_ncap ? A371249_ncap : 64;
    while (nc <= c)
        nc <<= 1;
    seq_t *t = mem_resize(seq_t, A371249_next, nc);
    if (!t)
        abort();
    for (size_t i = A371249_ncap; i < nc; ++i)
        t[i] = 0;
    A371249_next = t;
    A371249_ncap = nc;
}

static void A371249_step(void)
{
    size_t col = A371249_pos_in_diag + 1;
    A371249_grow_cols(col);
    size_t cand;
    if (A371249_next[col] == 0) {
        A371249_grow(A371249_smallest);
        while (A371249_pos[A371249_smallest])
            ++A371249_smallest, A371249_grow(A371249_smallest);
        cand = A371249_smallest;
    } else {
        cand = (size_t)A371249_next[col];
        A371249_grow(cand);
        while (A371249_pos[cand]) {
            cand += col;
            A371249_grow(cand);
        }
    }
    A371249_pos[cand] = ++A371249_count;
    A371249_next[col] = (seq_t)(cand + col);

    A371249_grow(A371249_smallest);
    while (A371249_pos[A371249_smallest]) {
        ++A371249_smallest;
        A371249_grow(A371249_smallest);
    }

    ++A371249_pos_in_diag;
    if (A371249_pos_in_diag == A371249_diag_sum - 1) {
        ++A371249_diag_sum;
        A371249_pos_in_diag = 0;
    }
}

seq_t a(idx_t n)
{
    size_t m = (size_t)n;
    if (m == 0)
        return 0;
    A371249_grow(m);
    while (A371249_pos[m] == 0)
        A371249_step();
    return (seq_t)A371249_pos[m];
}
