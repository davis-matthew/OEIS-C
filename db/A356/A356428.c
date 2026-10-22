/*
 * A356428  a(0) = a(1) = 0; for n > 1, a(n) is the number of distinct
 *          gpf(x)'s in the iterations x -> x - gpf(x) starting at n and
 *          ending at 0, where gpf = A006530.
 * https://oeis.org/A356428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=0; for n>1 let p = gpf(n), then a(n) = 1 + a(n-p)
 *            if p = n or gpf(n-p) > p, otherwise a(n) = a(n-p)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gpf.h"

static seq_t *cache   = NULL;
static bool  *seen    = NULL;
static size_t cap     = 0;

/* Grow the cache to hold indices up to 'needed' inclusive. */
static void grow_cache(size_t needed)
{
    if (needed <= cap)
        return;
    size_t newcap = (cap > 0 ? cap * 2 : 16);
    while (newcap <= needed)
        newcap <<= 1;
    seq_t *tmp_cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp_cache)
        abort();
    bool *tmp_seen  = (bool*)realloc(seen,  newcap * sizeof(bool));
    if (!tmp_seen)
        abort();
    for (size_t i = cap; i < newcap; ++i)
        tmp_seen[i] = false;
    cache = tmp_cache;
    seen  = tmp_seen;
    cap   = newcap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    grow_cache((size_t)n + 1);
    if (seen[n])
        return cache[n];
    uint64_t p64 = gpf((uint64_t)n);
    idx_t    p   = (idx_t)p64;
    idx_t    n_minus_p = n - p;
    idx_t    g = 0;
    if (n_minus_p > 0)
        g = (idx_t)gpf((uint64_t)n_minus_p);
    seq_t res;
    if (n_minus_p == 0 || g > p)
        res = 1 + a(n_minus_p);
    else
        res = a(n_minus_p);
    seen[n] = true;
    cache[n] = res;
    return res;
}
