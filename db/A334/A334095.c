/*
 * A334095  Primes p for which A329697(p) == 5.
 * https://oeis.org/A334095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. A329697 is completely additive with A329697(2)=0 and
 *            A329697(p) = 1 + A329697(p-1) for odd primes p. The code builds
 *            every odd number <= X with A329697 = j for j = 0..4 from primes
 *            q = 2^e*m+1. It then collects the primes p = 2^e*m + 1 <= X with
 *            m odd and A329697(m) = 4, sorts them, and d...
 * Range:     u8 n <= 10, u16 n <= 329, u32 n <= 1454, u64 n <= 1454,
 *            u128 n <= 1454, big any n
 * Validated: standard, pass, 78 terms through n = 1444 (2026-11-08)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

typedef struct { uint64_t *v; size_t n, cap; } A334095_vec;

static void A334095_push(A334095_vec *s, uint64_t x)
{
    if (s->n == s->cap) {
        s->cap = s->cap ? s->cap * 2 : 64;
        s->v = (uint64_t *)realloc(s->v, s->cap * sizeof(uint64_t));
    }
    s->v[s->n++] = x;
}

static int A334095_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return a < b ? -1 : (a > b ? 1 : 0);
}

static void A334095_sort_unique(A334095_vec *s)
{
    size_t i, k = 0;
    if (s->n == 0) return;
    qsort(s->v, s->n, sizeof(uint64_t), A334095_cmp);
    for (i = 0; i < s->n; i++)
        if (k == 0 || s->v[k - 1] != s->v[i]) s->v[k++] = s->v[i];
    s->n = k;
}

/* primes p <= X of the form 2^e*m + 1, e >= 1, m in src (odd) */
static void A334095_primes_from(const A334095_vec *src, uint64_t X, A334095_vec *out)
{
    size_t i;
    for (i = 0; i < src->n; i++) {
        uint64_t t = src->v[i];
        while (t <= (X - 1) / 2) {
            t *= 2;
            if (is_prime_u64(t + 1)) A334095_push(out, t + 1);
        }
    }
    A334095_sort_unique(out);
}

static uint64_t *A334095_found = NULL;
static size_t A334095_nfound = 0;

static void A334095_build(uint64_t X)
{
    A334095_vec N[5], P[5], R;
    int j, i;
    size_t x, y;
    for (j = 0; j < 5; j++) {
        N[j].v = NULL; N[j].n = N[j].cap = 0;
        P[j].v = NULL; P[j].n = P[j].cap = 0;
    }
    R.v = NULL; R.n = R.cap = 0;

    A334095_push(&N[0], 1);
    for (j = 1; j <= 4; j++) {
        A334095_primes_from(&N[j - 1], X, &P[j]);
        for (i = 1; i <= j; i++) {
            for (x = 0; x < P[i].n; x++) {
                uint64_t q = P[i].v[x];
                for (y = 0; y < N[j - i].n; y++) {
                    uint64_t r = N[j - i].v[y];
                    if (r <= X / q) A334095_push(&N[j], q * r);
                }
            }
        }
        A334095_sort_unique(&N[j]);
    }
    A334095_primes_from(&N[4], X, &R);

    for (j = 0; j < 5; j++) { free(N[j].v); free(P[j].v); }
    if (A334095_found) free(A334095_found);
    A334095_found = R.v;
    A334095_nfound = R.n;
}

seq_t a(idx_t n)
{
    static uint64_t X = 0;
    if (X == 0) {
        X = (uint64_t)1 << 16;
        A334095_build(X);
    }
    while ((idx_t)A334095_nfound < n && X < ((uint64_t)1 << 62)) {
        X <<= 2;
        A334095_build(X);
    }
    return (seq_t)A334095_found[n - 1];
}
