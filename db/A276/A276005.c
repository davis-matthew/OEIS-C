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
 * Validated: standard, pass, 98 terms through n = 4109 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of hit‑free factorial base numbers, computed by brute‑force search */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Return true if x has no "hits" in its factorial base representation. */
static bool hit_free(seq_t x)
{
    seq_t rem = x;
    seq_t base = 2;          /* base(i) = i+1, least significant digit position 1 */
    size_t i = 1;
    uint8_t digits[256] = {0};

    while (rem > 0) {
        seq_t digit = rem % base;
        digits[i] = (uint8_t)digit;
        rem /= base;
        ++i;
        ++base;
    }
    size_t len = i - 1;      /* number of digits */

    for (size_t k = 1; k <= len; ++k) {
        uint8_t d = digits[k];
        if (d != 0) {
            size_t j = k - d;
            if (j >= 1 && j <= len && digits[j] != 0)
                return false;
        }
    }
    return true;
}

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
