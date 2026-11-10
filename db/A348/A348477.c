/*
 * A348477  Drop all 1 but the first 1 in A035306.
 * https://oeis.org/A348477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   list the prime factorization of k = 1,2,3,... as p1, e1, p2, e2,
 *            ... (A035306), omitting each exponent equal to 1 except the
 *            leading 1 for k=1; a(n) is the n-th term of the concatenation
 * Range:     u8 n <= 596, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 132 terms through n = 9175 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A348477_cache = NULL;
static size_t A348477_cap = 0;
static size_t A348477_size = 0;
static uint64_t A348477_next = 0;

static void A348477_push(uint64_t v)
{
    if (A348477_size + 1 > A348477_cap) {
        size_t nc = A348477_cap == 0 ? 64 : A348477_cap * 2;
        seq_t *tmp = mem_resize(seq_t, A348477_cache, nc);
        if (!tmp) exit(EXIT_FAILURE);
        A348477_cache = tmp;
        A348477_cap = nc;
    }
    A348477_cache[A348477_size++] = (seq_t)v;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while ((uint64_t)A348477_size < (uint64_t)n) {
        ++A348477_next;
        uint64_t m = A348477_next;
        if (m == 1) { A348477_push(1); continue; }
        for (uint64_t p = 2; p * p <= m; ++p) {
            if (m % p != 0) continue;
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            A348477_push(p);
            if (e > 1) A348477_push(e);
        }
        if (m > 1) A348477_push(m);
    }
    return A348477_cache[(size_t)(n - 1)];
}
