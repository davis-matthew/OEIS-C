/*
 * A285317  Squarefree numbers n for which A019565(n) < n.
 * https://oeis.org/A285317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. Every term has a highest bit m, so k
 *            = 2^m + r with r < 2^m, and A019565(k) = prime(m+1)*A019565(r).
 *            For each m, a DFS enumerates the squarefree s = A019565(r) built
 *            from the first m primes with s*prime(m+1) < 2^(m+1). It keeps k
 *            when s*prime(m+1) < 2^m + r and k is squ...
 * Range:     u8 n <= 6, u16 n <= 226, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9783 (2026-11-08)
 * Helpers:   is_squarefree_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree_u64.h"

static const uint64_t A285317_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

static uint64_t *A285317_terms = NULL;
static size_t A285317_cnt = 0, A285317_cap = 0;
static int A285317_next_m = 1;

/* per-level state */
static int A285317_m;
static uint64_t A285317_P, A285317_top, A285317_lim;

static void A285317_push(uint64_t k)
{
    if (A285317_cnt == A285317_cap) {
        size_t nc = A285317_cap ? A285317_cap * 2 : 1024;
        uint64_t *t = (uint64_t *)realloc(A285317_terms, nc * sizeof(uint64_t));
        if (!t) return;
        A285317_terms = t;
        A285317_cap = nc;
    }
    A285317_terms[A285317_cnt++] = k;
}

static void A285317_record(uint64_t s, uint64_t r)
{
    if (s * A285317_P < A285317_top + r) {
        uint64_t k = A285317_top + r;
        if (is_squarefree_u64(k)) A285317_push(k);
    }
}

static void A285317_dfs(int i, uint64_t s, uint64_t r)
{
    uint64_t maxs = (A285317_lim - 1) / A285317_P; /* need s*P < lim */
    for (int j = i; j < A285317_m; ++j) {
        uint64_t p = A285317_primes[j];
        if (s > maxs / p) break;          /* s*p > maxs, primes increase */
        uint64_t ns = s * p;
        uint64_t nr = r | ((uint64_t)1 << j);
        A285317_record(ns, nr);
        A285317_dfs(j + 1, ns, nr);
    }
}

static int A285317_cmp(const void *x, const void *y)
{
    uint64_t a1 = *(const uint64_t *)x, b1 = *(const uint64_t *)y;
    return (a1 > b1) - (a1 < b1);
}

static void A285317_level(int m)
{
    size_t start = A285317_cnt;
    A285317_m = m;
    A285317_P = A285317_primes[m];
    A285317_top = (uint64_t)1 << m;
    A285317_lim = (uint64_t)1 << (m + 1);
    A285317_record(1, 0);
    A285317_dfs(0, 1, 0);
    if (A285317_cnt > start)
        qsort(A285317_terms + start, A285317_cnt - start, sizeof(uint64_t),
              A285317_cmp);
}

seq_t a(idx_t n)
{
    while ((uint64_t)A285317_cnt < (uint64_t)n && A285317_next_m <= 62) {
        A285317_level(A285317_next_m);
        ++A285317_next_m;
    }
    if ((uint64_t)n == 0 || (uint64_t)n > (uint64_t)A285317_cnt) return (seq_t)0;
    return (seq_t)A285317_terms[n - 1];
}
