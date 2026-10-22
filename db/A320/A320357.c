/*
 * A320357  a(0)=1; a(1)=1; for n >= 2, a(n) = a(n-1) + a(n-A000005(n)).
 * https://oeis.org/A320357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 66, u64 n <= 140,
 *            u128 n <= 296, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 1; a(1) = 1; 
 * for n ≥ 2, a(n) = a(n-1) + a(n-A000005(n)), where A000005(n) = number of divisors of n.
 */

#include <stdlib.h>

/* Dynamically allocated tables for divisor counts and sequence values. */
static uint16_t *divcnt = NULL;  /* divisor counts for 1..N */
static idx_t    divcnt_max = 0;  /* highest index for which divcnt is computed */
static seq_t   *cache   = NULL;  /* a(n) values for 0..N */
static idx_t    cache_max = 0;   /* highest index for which cache is computed */
static size_t   cache_sz = 0;    /* current capacity of cache array */

/* Ensure the divisor‑count table is large enough and contains counts up to needed_n. */
static void ensure_divcnt(idx_t needed_n)
{
    if (needed_n < divcnt_max) /* already up to date */
        return;

    /* Allocate or enlarge the array if necessary. */
    size_t new_sz = needed_n + 1;
    if (new_sz < 16) new_sz = 16;
    while (new_sz <= divcnt_max) new_sz <<= 1;

    uint16_t *tmp = (uint16_t*)realloc(divcnt, new_sz * sizeof(uint16_t));
    if (!tmp) abort();
    divcnt = tmp;
    divcnt_max = needed_n;

    /* Recompute divisor counts for 1..needed_n. */
    for (idx_t i = 1; i <= needed_n; ++i) divcnt[i] = 0;
    for (idx_t i = 1; i <= needed_n; ++i)
        for (idx_t j = i; j <= needed_n; j += i) ++divcnt[j];
}

/* Ensure the cache array is large enough and has computed values up to needed_n. */
static void ensure_cache(idx_t needed_n)
{
    if (needed_n <= cache_max) /* already computed */
        return;

    /* Allocate or enlarge the cache array if necessary. */
    if ((size_t)needed_n + 1 > cache_sz) {
        size_t new_sz = needed_n + 1;
        if (new_sz < 32) new_sz = 32;
        while (new_sz <= cache_sz) new_sz <<= 1;

        seq_t *tmp = (seq_t*)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_sz = new_sz;
    }

    /* Initialize base cases if this is the first call. */
    if (cache_max < 1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_max = 1;
    }

    /* Compute missing values in increasing order. */
    idx_t start = cache_max < 1 ? 2 : cache_max + 1;
    for (idx_t i = start; i <= needed_n; ++i) {
        idx_t idx_a = i - (idx_t)divcnt[i];   /* i - number of divisors of i */
        cache[i] = cache[i - 1] + cache[idx_a];
    }
    cache_max = needed_n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    ensure_divcnt(n);
    ensure_cache(n);
    return cache[n];
}
