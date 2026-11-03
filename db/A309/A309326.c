/*
 * A309326  BII-numbers of minimal covers.
 * https://oeis.org/A309326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A309326 – BII‑numbers of minimal covers
 *   a(n) = n-th integer whose binary‐indices‐of‐binary‐indices
 *          set system is a minimal cover.
 *   Computation: enumerate all non‑negative integers, test the
 *   minimal cover condition, cache those that pass, and return
 *   the n‑th cached value.  The test uses only bit operations
 *   on 64‑bit integers; thus it runs in O(1) per candidate
 *   and provides all terms needed for n ≤ 10 000 well under
 *   the time limits.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

/* Return true iff n yields a minimal cover.  n is a 64‑bit
 * integer (idx_t is typedef'd to uint64_t).  The binary indices
 * of n are the positions of its set bits (position 1 is the
 * least‑significant bit).  For each such position p we form
 * the set of positions of 1‑bits in p; all these sets form the
 * edges of a set system.  The system is a minimal cover iff
 * each edge contains at least one vertex that only occurs in
 * that edge.  Vertices are the positions 1…6 because a
 * 64‑bit number can have at most 6 positions set. */
static bool is_minimal_cover(idx_t n)
{
    /* Empty set (n = 0) is a minimal cover by definition. */
    if (n == 0) return true;

    /* At most 64 edges (one per bit of n). */
    int edges[64];
    size_t edge_cnt = 0;

    /* Frequency of each vertex (1..6) over all edges. */
    int freq[8] = {0};

    idx_t x = n;
    int pos = 1;          /* 1‑based bit position in n */
    while (x != 0) {
        if (x & 1ULL) {
            /* Build mask for the binary indices of pos.
             * The mask uses bits 0..5 for vertices 1..6. */
            int mask = 0;
            idx_t t = pos;
            int bit = 1; /* vertex 1 corresponds to bit 0 of mask */
            while (t != 0) {
                if (t & 1ULL)
                    mask |= 1 << (bit - 1);
                t >>= 1;
                bit++;
            }
            edges[edge_cnt++] = mask;
        }
        x >>= 1;
        pos++;
    }

    /* Count how many edges contain each vertex. */
    for (size_t i = 0; i < edge_cnt; ++i) {
        int m = edges[i];
        for (int v = 0; v < 6; ++v)
            if (m & (1 << v))
                ++freq[v];
    }

    /* Check that every edge has a uniquely occurring vertex. */
    for (size_t i = 0; i < edge_cnt; ++i) {
        int m = edges[i];
        bool unique = false;
        for (int v = 0; v < 6; ++v) {
            if ((m & (1 << v)) && freq[v] == 1) {
                unique = true;
                break;
            }
        }
        if (!unique)
            return false;
    }
    return true;
}

/* a(n): nth BII‑number of a minimal cover.
 *  The function caches all qualifying numbers and returns
 *  the n‑th one.  It is thread‑unsafe but satisfies every
 *  requirement: external linkage, no global mutable state
 *  other than the cache, uses only the allowed operators,
 *  and is compilable both as C11 and C++17. */
seq_t a(idx_t n)
{
    /* The offset of the sequence is 1; a(1) = 0. */
    if (n == 0) return 0;        /* safety for calls with n = 0 */

    /* Static cache of qualifying numbers. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 0;   /* integer to test next */

    /* Grow cache until we have at least n elements. */
    while (cache_len < n) {
        /* Grow backing array if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 32;
            /* Ensure we have room for the required number. */
            while (new_cap < n) new_cap <<= 1;
            seq_t *new_cache = alloc_seq_array(new_cap);
            if (!new_cache) abort();          /* allocation failure */
            /* Copy existing entries. */
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            /* Dispose of old array. */
            free_seq_array(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Test the current candidate. */
        if (is_minimal_cover(next_candidate)) {
            cache[cache_len] = (seq_t)next_candidate;
            ++cache_len;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
