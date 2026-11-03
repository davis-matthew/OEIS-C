/*
 * A297256  Numbers whose base-5 digits have equal down-variation and
 *          up-variation; see Comments.
 * https://oeis.org/A297256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9420 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the nth positive integer whose base‑5
   up‑variation equals its down‑variation. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true if the base‑5 representation of val has equal down and up
   variations.  The function works for any seq_t that supports % and / 5. */
static bool has_equal_variation(seq_t val)
{
    /* Base‑5 digits; 64 is enough for all seq_t that fit in 128 bits. */
    uint8_t digits[64];
    size_t len = 0;
    seq_t x = val;

    if (x == 0) {            /* not expected for positive integers */
        return true;
    }

    /* Extract digits from least‑significant to most‑significant. */
    while (x != 0) {
        digits[len++] = (uint8_t)(x % 5);
        x /= (seq_t)5;
    }

    unsigned int dv = 0;
    unsigned int uv = 0;
    /* Iterate from most‑significant to least‑significant digit. */
    unsigned char prev = digits[len - 1];
    for (int i = (int)len - 2; i >= 0; --i) {
        unsigned char curr = digits[i];
        int diff = (int)curr - (int)prev;
        if (diff > 0) {
            dv += (unsigned int)diff;
        } else if (diff < 0) {
            uv += (unsigned int)(-diff);
        }
        prev = curr;
    }
    return dv == uv;
}

/* Cache for previously computed terms */
static seq_t *cache = NULL;        /* array of values */
static size_t cache_len = 0;       /* number of cached terms */
static size_t cache_cap = 0;       /* capacity of the array */
static seq_t next_candidate = 1;   /* next integer to test */

/* Append a new value to the cache, growing the array if necessary. */
static void append_cache(seq_t value)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *new_cache = mem_new(seq_t, new_cap);
        if (!new_cache) {
            /* Allocation failed – terminate program (no safety path required). */
            abort();
        }
        for (size_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        mem_free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = value;
}

/* Public sequence function.  Returns the nth term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* undefined for n = 0, but safe return */
    }

    /* Ensure the cache contains at least n terms. */
    while (cache_len < n) {
        while (!has_equal_variation(next_candidate)) {
            next_candidate++;
        }
        append_cache(next_candidate);
        next_candidate++;   /* move to next number for subsequent searches */
    }
    return cache[n - 1];
}
