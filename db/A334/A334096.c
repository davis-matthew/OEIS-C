/*
 * A334096  Primes p for which A329697(p) == 6.
 * https://oeis.org/A334096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. A329697 is completely additive with f(2) = 0 and * f(p)
 *            = 1 + f(p-1) for odd primes p, so a(n) is the n-th prime of the
 *            * form m + 1 with m = 2^j * o, j >= 1, o odd, f(o) = 5 (m is in
 *            A334105). * The odd numbers with f = v are built level by level
 *            as products of primes * with f <= v, up t...
 * Range:     u8 none, u16 n <= 631, u32 n <= 4827, u64 n <= 4827,
 *            u128 n <= 4827, big any n
 * Validated: standard, pass, 77 terms through n = 4789 (2026-11-08)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

typedef struct { uint64_t *v; size_t n, cap; } A334096_list;

static void A334096_push(A334096_list *l, uint64_t x)
{
    if (l->n == l->cap) {
        l->cap = l->cap ? 2 * l->cap : 64;
        uint64_t *t = (uint64_t *)realloc(l->v, l->cap * sizeof *t);
        if (!t) abort();
        l->v = t;
    }
    l->v[l->n++] = x;
}

typedef struct { uint64_t p; int f; } A334096_pf;

static int A334096_cmp_pf(const void *x, const void *y)
{
    uint64_t a = ((const A334096_pf *)x)->p, b = ((const A334096_pf *)y)->p;
    return (a > b) - (a < b);
}

static int A334096_cmp_u64(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

static A334096_pf *A334096_PR = NULL;
static size_t A334096_npr = 0, A334096_prcap = 0;
static uint64_t A334096_lim;

static void A334096_products(size_t start, int budget, uint64_t prod, A334096_list *out)
{
    if (budget == 0) { A334096_push(out, prod); return; }
    for (size_t i = start; i < A334096_npr; ++i) {
        if (A334096_PR[i].p > A334096_lim / prod) break;
        if (A334096_PR[i].f <= budget)
            A334096_products(i, budget - A334096_PR[i].f, prod * A334096_PR[i].p, out);
    }
}

static seq_t *A334096_T = NULL;
static size_t A334096_cnt = 0;
static uint64_t A334096_L = 0;

static void A334096_build(uint64_t bound)
{
    A334096_lim = bound;
    A334096_list odd[6];
    for (int v = 0; v < 6; ++v) { odd[v].v = NULL; odd[v].n = 0; odd[v].cap = 0; }
    A334096_push(&odd[0], 1);
    A334096_npr = 0;
    for (int v = 1; v <= 5; ++v) {
        for (size_t i = 0; i < odd[v - 1].n; ++i)
            for (uint64_t x = 2 * odd[v - 1].v[i]; x < bound; x *= 2) {
                if (is_prime_u64(x + 1)) {
                    if (A334096_npr == A334096_prcap) {
                        A334096_prcap = A334096_prcap ? 2 * A334096_prcap : 256;
                        A334096_pf *t = (A334096_pf *)realloc(A334096_PR,
                                            A334096_prcap * sizeof *t);
                        if (!t) abort();
                        A334096_PR = t;
                    }
                    A334096_PR[A334096_npr].p = x + 1;
                    A334096_PR[A334096_npr].f = v;
                    ++A334096_npr;
                }
                if (x > bound / 2) break;
            }
        qsort(A334096_PR, A334096_npr, sizeof *A334096_PR, A334096_cmp_pf);
        A334096_products(0, v, 1, &odd[v]);
    }
    /* primes with f = 6: p = 2^j o + 1, j >= 1, f(o) = 5, p <= bound */
    A334096_list t; t.v = NULL; t.n = 0; t.cap = 0;
    for (size_t i = 0; i < odd[5].n; ++i)
        for (uint64_t x = 2 * odd[5].v[i]; x < bound; x *= 2) {
            if (is_prime_u64(x + 1)) A334096_push(&t, x + 1);
            if (x > bound / 2) break;
        }
    if (t.n) qsort(t.v, t.n, sizeof *t.v, A334096_cmp_u64);
    mem_free(A334096_T);
    A334096_T = mem_new(seq_t, t.n ? t.n : 1);
    if (!A334096_T) abort();
    for (size_t i = 0; i < t.n; ++i) A334096_T[i] = (seq_t)t.v[i];
    A334096_cnt = t.n;
    for (int v = 0; v <= 5; ++v) free(odd[v].v);
    free(t.v);
    A334096_L = bound;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A334096_cnt < (size_t)n)
        A334096_build(A334096_L ? 2 * A334096_L : (uint64_t)1 << 16);
    return A334096_T[n - 1];
}
