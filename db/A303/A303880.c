/*
 * A303880  Lexicographically earliest sequence of distinct terms such that
 *          a(n) shares at least one even digit with a(n+1).
 * https://oeis.org/A303880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 200, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   shares_even_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: greedy search of the smallest unused integer that shares an even
   decimal digit with the previous term. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/shares_even_digit.h"

/* ------------------------------------------------------------------ */
/*  Memoisation state                                                */
/* ------------------------------------------------------------------ */

static seq_t *cache     = NULL;   /* a(n) in 0‑based order                */
static size_t cache_len = 0;      /* number of terms currently cached      */

/* Bitset of used integers.  Index 0 is unused; values are 1‑based. */
static uint8_t *used   = NULL;
static size_t   used_cap = 0;     /* allocated size of used array        */

/* Ensure the used bitset can hold at least `cap` entries. */
static void ensure_used_cap(size_t cap)
{
    if (used_cap >= cap)
        return;
    size_t new_cap = used_cap ? used_cap << 1 : 64;
    while (new_cap < cap)
        new_cap <<= 1;
    uint8_t *p = (uint8_t *)realloc(used, new_cap);
    if (!p)
        abort();
    /* Initialise newly added entries to zero. */
    for (size_t i = used_cap; i < new_cap; ++i)
        p[i] = 0;
    used = p;
    used_cap = new_cap;
}

/* Mark integer `v` (assumed to fit in uint64_t) as used. */
static void mark_used(uint64_t v)
{
    ensure_used_cap(v + 1);     /* need an entry for v itself */
    used[v] = 1;
}

/* Return true if integer `v` (fits in uint64_t) has already been used. */
static bool already_used(uint64_t v)
{
    if (v >= used_cap)
        return false;
    return used[v] != 0;
}

/* Extend the cache up to index `n` (1‑based). */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    /* Initialise a(1)=2 if empty. */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache)
            abort();
        cache[0] = (seq_t)2;
        cache_len = 1;
        mark_used(2);
    }

    /* Generate subsequent terms. */
    while (cache_len < (size_t)n) {
        seq_t prev = cache[cache_len - 1];
        uint64_t cand = 1;
        /* Find the smallest unused integer that shares an even digit with prev. */
        while (true) {
            if (!already_used(cand) && shares_even_digit(prev, (seq_t)cand))
                break;
            ++cand;
        }
        /* Append the new term. */
        seq_t *tmp = (seq_t *)realloc(cache,
                      (cache_len + 1) * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache[cache_len] = (seq_t)cand;
        ++cache_len;
        mark_used(cand);
    }
}

/* ------------------------------------------------------------------ */
/*  Public interface                                                 */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1)
        abort();                    /* undefined for n = 0 */
    ensure_cache(n);
    return cache[n - 1];
}
