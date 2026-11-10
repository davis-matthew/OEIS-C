/*
 * A331435  a(n) is the least positive k such that A028916(n) - k^2 is a
 *          fourth power.
 * https://oeis.org/A331435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. List every prime p = x^2 + b^4 <= L,
 *            with x, b >= 1. For each such p, a(n) is the smallest x. The
 *            primes are sorted to give A028916(n), and L is doubled until
 *            there are at least n of them
 * Range:     u8 n <= 445, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 8252 (2026-11-08)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

typedef struct { uint64_t p; uint64_t x; } A331435_pair;

static int A331435_cmp(const void *s, const void *t)
{
    const A331435_pair *u = (const A331435_pair *)s;
    const A331435_pair *v = (const A331435_pair *)t;
    if (u->p != v->p) return u->p < v->p ? -1 : 1;
    if (u->x != v->x) return u->x < v->x ? -1 : 1;
    return 0;
}

seq_t a(idx_t n)
{
    static uint64_t *ks = NULL;
    static size_t cnt = 0;
    static uint64_t limit = 1024;

    while (cnt < (size_t)n)
    {
        size_t cap = 1024, m = 0;
        A331435_pair *v = (A331435_pair *)malloc(cap * sizeof(A331435_pair));
        if (!v) abort();
        for (uint64_t b = 1; b * b * b * b < limit; ++b)
        {
            uint64_t b4 = b * b * b * b;
            for (uint64_t x = 1; b4 + x * x <= limit; ++x)
            {
                uint64_t p = b4 + x * x;
                if (p > 2 && (p & 1) == 0) continue;
                if (!is_prime((idx_t)p)) continue;
                if (m == cap)
                {
                    cap *= 2;
                    v = (A331435_pair *)realloc(v, cap * sizeof(A331435_pair));
                    if (!v) abort();
                }
                v[m].p = p;
                v[m].x = x;
                ++m;
            }
        }
        qsort(v, m, sizeof(A331435_pair), A331435_cmp);
        free(ks);
        ks = (uint64_t *)malloc((m + 1) * sizeof(uint64_t));
        if (!ks) abort();
        cnt = 0;
        for (size_t i = 0; i < m; ++i)
        {
            if (i == 0 || v[i].p != v[i - 1].p)
                ks[cnt++] = v[i].x;
        }
        free(v);
        if (cnt < (size_t)n) limit *= 2;
    }
    return (seq_t)ks[n - 1];
}
