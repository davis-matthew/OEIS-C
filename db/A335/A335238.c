/*
 * A335238  Numbers k such that the distinct parts of the k-th composition in
 *          standard order (A066099) are not pairwise coprime, where a
 *          singleton is not coprime unless it is (1).
 * https://oeis.org/A335238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Enumerate non‑negative integers k, keep those whose
 * distinct parts of the k‑th standard composition (A066099) are not
 * pairwise coprime (a singleton >1 is counted as not coprime).
 * The n‑th value returned is the n‑th such k (a(1)=0).
 *
 * This uses a cache so that any order of calls works.  The computation
 * is O(m) where m is the maximal n requested (up to about 10 000 in
 * the test data).  It uses the provided gcd(seq_t,seq_t) helper.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"   /* gcd(seq_t a, seq_t b) */

/* Static cache for the sequence values */
static seq_t *cache = NULL;   /* array of found k’s              */
static size_t cache_len = 0;  /* number of cached elements       */
static size_t cache_cap = 0;  /* capacity of the array           */
static idx_t next_k = 0;      /* next k to test                  */

/* Ensure the cache is large enough.  On first use allocate some space. */
static void
ensure_cache_size(size_t needed)
{
    if (cache_len + needed <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_cap < cache_len + needed)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Test whether the standard-order composition of k fails the coprime test.
 * Returns true if it should be included in the sequence (i.e. fails). */
static bool
composition_fails(idx_t k)
{
    /* Positions of the set bits in k (1‑based). Each position gives a
     * part difference from the previous position.  We keep only distinct
     * parts. */
    int distinct_parts[64];
    size_t distinct_len = 0;
    int last_pos    = 0;
    uint64_t mask   = (uint64_t)k; /* idx_t is uint64_t */

    while (mask != 0) {
        int pos  = __builtin_ctzll(mask) + 1; /* 1‑based position of lowest 1 bit */
        int diff = pos - last_pos;            /* part size             */
        bool new_part = true;
        for (size_t i = 0; i < distinct_len; ++i)
            if (distinct_parts[i] == diff) { new_part = false; break; }
        if (new_part)
            distinct_parts[distinct_len++] = diff;
        last_pos = pos;
        mask &= mask - 1; /* remove lowest set bit */
    }

    /* Decide if this composition fails the pairwise coprime condition. */
    if (distinct_len == 0) {          /* empty composition */
        return true;
    }
    if (distinct_len == 1) {          /* singleton */
        return distinct_parts[0] > 1;
    }

    for (size_t i = 0; i < distinct_len; ++i) {
        for (size_t j = i + 1; j < distinct_len; ++j) {
            seq_t g = gcd((seq_t)distinct_parts[i], (seq_t)distinct_parts[j]);
            if (g > (seq_t)1)
                return true;          /* found non‑coprime pair */
        }
    }
    return false;                     /* all distinct parts pairwise coprime */
}

seq_t a(idx_t n)
{
    /* Generate results until we have at least n cached values. */
    while (cache_len < n) {
        if (composition_fails(next_k)) {
            ensure_cache_size(1);
            cache[cache_len++] = (seq_t)next_k;
        }
        next_k++;
    }
    return cache[n - 1];
}
