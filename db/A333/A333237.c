/*
 * A333237  Numbers k such that 1/k contains at least one '9' in its decimal
 *          expansion.
 * https://oeis.org/A333237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains9decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for integers whose 1/k decimal expansion contains at least one
   digit '9'.  The function builds a cache of such numbers on demand
   and returns the n-th term (1‑based index). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains9decimal.h"

/* Global state for memoisation. */
static seq_t *cache = NULL;
static size_t cache_len = 0;        /* number of values stored        */
static size_t cache_cap = 0;        /* allocated capacity             */
static uint64_t last_checked = 1;   /* last k that was examined */

/* Ensure enough space in cache.  The cast is needed to compile under
   C++ (void* is not implicitly convertible to seq_t*). */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!new_cache)
        abort();                   /* memory allocation failed     */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Generate terms up to n (1‑based). */
static void generate_up_to(idx_t n)
{
    while (cache_len < n) {
        uint64_t k = ++last_checked;
        if (contains9decimal(k)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for offset 0, harmless */
    generate_up_to(n);
    return cache[n - 1];
}
