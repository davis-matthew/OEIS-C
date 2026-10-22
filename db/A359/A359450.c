/*
 * A359450  a(1) = 1, a(2) = 2; thereafter a(n) = n * a(A070939(n)).
 * https://oeis.org/A359450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * a(A070939(n)), where A070939(n) = floor(log2(n)) + 1
 *            (i.e. the number of bits needed to represent n in binary).
 *            Initial values: a(1) = 1, a(2) = 2
 * Range:     u8 n <= 10, u16 n <= 303, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   num_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/num_bits.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure internal cache can hold values up to index n */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) return; /* Allocation failed; let caller handle */
        cache[1] = (seq_t)1;
        if (cache_cap > 2) cache[2] = (seq_t)2;
        return;
    }
    if ((idx_t)n >= (idx_t)cache_cap) {
        size_t new_cap = cache_cap * 2;
        while ((idx_t)new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return; /* Allocation failed; caller will crash */
        cache = tmp;
        memset(cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache_cap = new_cap;
        if (cache[1] == 0) cache[1] = (seq_t)1;
        if (cache_cap > 2 && cache[2] == 0) cache[2] = (seq_t)2;
    }
}

/* Recursive computation with memoisation */
static seq_t compute_a(idx_t n)
{
    ensure_cache(n);
    if (cache[(size_t)n] != 0) return cache[(size_t)n];
    idx_t bits = num_bits(n);
    seq_t val = (seq_t)n * compute_a(bits);
    cache[(size_t)n] = val;
    return val;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    return compute_a(n);
}
