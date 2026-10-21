/*
 * A276005  Numbers with hit-free factorial base representations; positions of
 *          zeros in A276004 & A276007.
 * https://oeis.org/A276005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 4140, u32 n <= 4140, u64 n <= 4140,
 *            u128 n <= 4140, big any n
 * Helpers:   hit_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of hit‑free factorial base numbers, computed by brute‑force search */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/hit_free.h"

#if defined(__cplusplus)
static seq_t *cache = nullptr;
#else
static seq_t *cache = NULL;
#endif
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_test = 0;

/* Ensure cache contains at least a(n) for all n up to 'need'. */
static void grow_cache_until(idx_t need)
{
    while (cache_len <= (size_t)need) {
        seq_t val;
        for (;;) {
            val = (seq_t)next_test++;
            if (hit_free(val))
                break;
        }

        if (cache_len == cache_cap) {
            size_t newCap = cache_cap ? (cache_cap << 1) : 16;
            if (newCap < cache_len + 1) newCap = cache_len + 1;
#if defined(__cplusplus)
            seq_t *newCache = new seq_t[newCap];
            for (size_t i = 0; i < cache_len; ++i)
                newCache[i] = cache[i];
            delete[] cache;
            cache = newCache;
#else
            seq_t *tmp = realloc(cache, newCap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
#endif
            cache_cap = newCap;
        }
        cache[cache_len++] = val;
    }
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    grow_cache_until(n);
    return cache[n];
}
