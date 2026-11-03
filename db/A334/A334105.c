/*
 * A334105  Numbers m for which A329697(m) = 5.
 * https://oeis.org/A334105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 4325, u32 n <= 50071, u64 n <= 50071,
 *            u128 n <= 50071, big any n
 * Validated: standard, pass, 83 terms through n = 9806 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A329697 is completely additive with f(2) = 0 and f(p) = 1 + f(p - 1) for
 * odd primes p. So up to a bound L, level by level for v = 1..5:
 *   the primes with f = v are the primes 2^j o + 1 (j >= 1) for the odd o
 *   with f(o) = v - 1, and
 *   the odd numbers with f = v are the products of such primes (with
 *   repetition) whose f's add up to v;
 * and the terms are the 2^j o <= L with o odd, f(o) = 5, sorted. L doubles
 * until there are n. (The original factored every m in turn, with a hash
 * table of step counts; the terms pass 2e9, so it timed out.) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

typedef struct { uint64_t *v; size_t n, cap; } list;

static void push(list *l, uint64_t x)
{
    if (l->n == l->cap) {
        l->cap = l->cap ? 2 * l->cap : 64;
        uint64_t *t = (uint64_t *)realloc(l->v, l->cap * sizeof *t);
        if (!t) abort();
        l->v = t;
    }
    l->v[l->n++] = x;
}

static int cmp_u64(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

static uint64_t PR[4096];                     /* the primes with f <= 5, ascending */
static int PF[4096], npr;                     /* and their f */
static uint64_t lim;

static void products(int start, int budget, uint64_t prod, list *out)
{
    if (budget == 0) { push(out, prod); return; }
    for (int i = start; i < npr; ++i) {
        if (PR[i] > lim / prod) break;        /* ascending, so all further are too big */
        if (PF[i] <= budget) products(i, budget - PF[i], prod * PR[i], out);
    }
}

static seq_t *T = NULL;
static size_t cnt = 0;
static uint64_t L = 0;

static void build(uint64_t bound)
{
    lim = bound;
    list odd[6] = {{0}};
    push(&odd[0], 1);
    npr = 0;
    for (int v = 1; v <= 5; ++v) {
        for (size_t i = 0; i < odd[v - 1].n; ++i)
            for (uint64_t x = 2 * odd[v - 1].v[i]; x < lim; x *= 2) {
                if (is_prime_u64(x + 1)) {
                    if (npr == 4096) abort();
                    PR[npr] = x + 1;
                    PF[npr++] = v;
                }
                if (x > lim / 2) break;
            }
        for (int i = 1; i < npr; ++i)         /* keep PR ascending (with PF) */
            for (int j = i; j > 0 && PR[j - 1] > PR[j]; --j) {
                uint64_t t = PR[j]; PR[j] = PR[j - 1]; PR[j - 1] = t;
                int f = PF[j]; PF[j] = PF[j - 1]; PF[j - 1] = f;
            }
        products(0, v, 1, &odd[v]);
    }
    list t = {0};
    for (size_t i = 0; i < odd[5].n; ++i)
        for (uint64_t m = odd[5].v[i]; m <= lim; m *= 2) {
            push(&t, m);
            if (m > lim / 2) break;
        }
    qsort(t.v, t.n, sizeof *t.v, cmp_u64);
    mem_free(T);
    T = mem_new(seq_t, t.n ? t.n : 1);
    if (!T) abort();
    for (size_t i = 0; i < t.n; ++i) T[i] = (seq_t)t.v[i];
    cnt = t.n;
    for (int v = 0; v <= 5; ++v) free(odd[v].v);
    free(t.v);
    L = bound;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (cnt < (size_t)n) build(L ? 2 * L : (uint64_t)1 << 20);
    return T[n - 1];
}
