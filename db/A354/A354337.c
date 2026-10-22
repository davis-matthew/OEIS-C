/*
 * A354337  a(n) is the integer w such that (L(2*n)^2, -L(2*n + 1)^2, w) is a
 *          primitive solution to the Diophantine equation 2*x^3 + 2*y^3 + z^3
 *          = 125, where L(n) is the n-th Lucas number (A000032).
 * https://oeis.org/A354337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=19, a(2)=149, a(3)=1039, a(n)=8*a(n-1)-8*a(n-2)+a(n-3) for n≥4 */
#include <stdlib.h>

/* static cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_sz = 0;  /* number of elements in cache (cache[0] = a(1)) */

/* ensure that cache contains at least the first `n` terms */
static void ensure_terms(idx_t n)
{
    if (n == 0)
        return;

    /* initialise the first three terms on first use */
    if (cache_sz == 0) {
        cache_sz = 3;
        cache = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)19;    /* a(1) */
        cache[1] = (seq_t)149;   /* a(2) */
        cache[2] = (seq_t)1039;  /* a(3) */
    }

    size_t need = (size_t)n;
    if (cache_sz >= need)
        return;

    /* grow the cache and compute new terms */
    seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;

    for (size_t i = cache_sz; i < need; ++i) {
        /* idx mapping: cache[0] = a(1), cache[1] = a(2), …  */
        seq_t val = 8 * cache[i-1] - 8 * cache[i-2] + cache[i-3];
        cache[i] = val;
    }
    cache_sz = need;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* behaviour below the offset is irrelevant */
    ensure_terms(n);
    return cache[(size_t)n - 1];
}
