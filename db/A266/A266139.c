/*
 * A266139  Volcano palindromes: palindromes not in A110784 such that removing
 *          one digit will result in a term in A110784.
 * https://oeis.org/A266139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 260, u32 n <= 5158, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9896 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Gen-Volcano: enumerates odd‑length palindromes whose left portion is
 * non‑decreasing and whose middle digit is strictly smaller than its
 * neighbours. The first ten terms are the one‑digit numbers 0‑9.
 * The numbers are produced in increasing numeric order, exactly as
 * listed in OEIS A266139.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static void push_cache(seq_t val, seq_t **cptr, size_t *cap, size_t *size)
{
    if (*size + 1 >= *cap) {
        size_t new_cap = (*cap) ? (*cap << 1) : 32;
        seq_t *tmp = mem_resize(seq_t, *cptr, new_cap);
        if (!tmp) exit(1);          /* allocation failure → abort */
        *cptr = tmp;
        *cap  = new_cap;
    }
    (*cptr)[++(*size)] = val;
}

static void generate_k(size_t k, int start, int pos, int *left,
                       seq_t **cptr, size_t *cap, size_t *size)
{
    if (pos == (int)k) {
        int last = left[k-1];
        for (int a = 0; a < last; ++a) {
            seq_t v = 0;
            for (size_t i = 0; i < k; ++i)
                v = v * 10 + (seq_t)left[i];
            v = v * 10 + (seq_t)a;
            for (int i = (int)k-1; i >= 0; --i)
                v = v * 10 + (seq_t)left[i];
            push_cache(v, cptr, cap, size);
        }
        return;
    }
    for (int d = start; d <= 9; ++d) {
        left[pos] = d;
        generate_k(k, d, pos+1, left, cptr, cap, size);
    }
}

/* forward declarations of helper functions */
static void push_cache(seq_t val, seq_t **cache, size_t *cap, size_t *size);
static void generate_k(size_t k, int start_digit, int pos, int *left,
                       seq_t **cache, size_t *cap, size_t *size);

/* static state shared between calls */
static seq_t  *cache   = NULL;
static size_t  cache_cap = 0;
static size_t  cache_sz  = 0;
static size_t  cur_k     = 0;      /* length of left side processed */

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 32;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return 0;
        /* one‑digit numbers 0‑9 */
        for (idx_t i = 0; i < 10; ++i)
            cache[i+1] = (seq_t)i;
        cache_sz = 10;
    }

    /* generate further terms until we have at least n of them */
    while (cache_sz < (size_t)n) {
        ++cur_k;                       /* left side length to generate */
        int left[32];
        generate_k(cur_k, 1, 0, left, &cache, &cache_cap, &cache_sz);
    }
    return cache[n];
}
