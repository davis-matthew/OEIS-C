/*
 * A303657  a(n) is the least positive integer not yet in the sequence which
 *          shares a digit with a(n-2); a(1)=0, a(2)=1.
 * https://oeis.org/A303657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A303657: a(1)=0, a(2)=1; a(n)=least positive integer not yet in the sequence which shares a digit with a(n-2) */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_counts.h"

/* Static cache of computed terms and a list of used numbers */
static seq_t *cache = NULL;
static size_t cache_sz = 0, cache_cap = 0;

static seq_t *used = NULL;
static size_t used_sz = 0, used_cap = 0;

/* Ensure cache storage */
static void ensure_cache_cap(size_t need) {
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        if (new_cap < need) new_cap = need;
        seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

/* Ensure used numbers storage */
static void ensure_used_cap(size_t need) {
    if (need > used_cap) {
        size_t new_cap = used_cap ? used_cap << 1 : 4;
        if (new_cap < need) new_cap = need;
        seq_t *new_ptr = (seq_t*)realloc(used, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();
        used = new_ptr;
        used_cap = new_cap;
    }
}

/* Linear search for presence */
static bool in_used(seq_t v) {
    for (size_t i = 0; i < used_sz; ++i)
        if (used[i] == v) return true;
    return false;
}

/* Append to used list */
static void add_used(seq_t v) {
    ensure_used_cap(used_sz + 1);
    used[used_sz++] = v;
}

/* Append to cache */
static void add_cache(seq_t v) {
    ensure_cache_cap(cache_sz + 1);
    cache[cache_sz++] = v;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n<1, but not expected */

    /* Initialize storage if needed */
    if (!cache) {
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    if (!used) {
        used_cap = 8;
        used = (seq_t*)malloc(used_cap * sizeof(seq_t));
    }

    /* Ensure base terms are present */
    while (cache_sz < 2 && cache_sz < (size_t)n) {
        seq_t val = cache_sz == 0 ? 0 : 1;
        add_cache(val);
        add_used(val);
    }

    /* Build sequence up to required index */
    while (cache_sz < (size_t)n) {
        seq_t prev2 = cache[cache_sz - 2];          /* a(n-2) */
        unsigned char prevcnt[10];
        digit_counts(prev2, prevcnt);              /* digits of a(n-2) */

        seq_t cand = 1;                            /* search starts at 1 */
        while (true) {
            if (!in_used(cand)) {
                unsigned char candcnt[10];
                digit_counts(cand, candcnt);       /* digits of candidate */

                bool share = false;
                for (int d = 0; d < 10; ++d)
                    if (prevcnt[d] && candcnt[d]) { share = true; break; }

                if (share) break;                  /* found next term */
            }
            ++cand;
        }

        add_cache(cand);
        add_used(cand);
    }

    return cache[n - 1];
}
