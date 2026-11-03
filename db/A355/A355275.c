/*
 * A355275  Numbers having more odd than even digits when written in base 3.
 * https://oeis.org/A355275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 7441, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9827 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A355275: Numbers having more odd than even digits when written in base 3.
   The function builds the sequence incrementally: it scans natural numbers in order,
   keeps the ones that satisfy the digit‑balance condition, and stops once the requested
   term has been found.  This works efficiently for n up to about 10 000. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool has_more_odd(seq_t x)
{
    int odd = 0, even = 0;
    while (x) {
        unsigned int d = (unsigned int)(x % 3);
        if (d == 1) odd++; else even++;
        x /= 3;
    }
    return odd > even;
}

static seq_t *cache = NULL;              /* stored sequence values        */
static size_t cache_len = 0;             /* number of cached terms         */
static size_t cache_cap = 0;             /* allocated capacity              */
static idx_t next_candidate = 1;         /* next integer to test          */
static bool initialized = false;        /* ensure 1st‑time setup         */

seq_t a(idx_t n)
{
    /* Lazily initialize the cache if this is the first call. */
    if (!initialized) {
        cache_cap = 8;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) abort();             /* simple error handling          */
        initialized = true;
    }

    /* Extend the cache until we have at least n terms. */
    while (cache_len < n) {
        seq_t val = (seq_t)next_candidate;
        if (has_more_odd(val)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp) abort();        /* simple error handling          */
                cache = tmp;
            }
            cache[cache_len++] = val;
        }
        next_candidate++;
    }
    return cache[n - 1];
}
