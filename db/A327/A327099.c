/*
 * A327099  BII-numbers of set-systems with non-spanning edge-connectivity 1.
 * https://oeis.org/A327099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th BII‑number of a set‑system with non‑spanning
 *            edge‑connectivity 1. The function enumerates integers, tests the
 *            property as defined in the OEIS entry, and memoises the results
 * Range:     u8 n <= 45, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Simple union–find helpers on the 0..63 vertex range. */
static inline int uf_find(int v, int *parent)
{
    while (parent[v] != v) {
        parent[v] = parent[parent[v]];
        v = parent[v];
    }
    return v;
}
static inline void uf_union(int a, int b, int *parent)
{
    int fa = uf_find(a, parent);
    int fb = uf_find(b, parent);
    if (fa != fb) parent[fa] = fb;
}

/* Test whether the BII-number n has a set‑system with
   non‑spanning edge‑connectivity 1.  n is treated as a sequence element,
   i.e. a sequence integer, not an index. */
static bool is_special(seq_t n)
{
    if (n == (seq_t)0) return false;

    /* Build the list of edges.  Each edge is a bit mask of vertices
       (positions of 1’s in the binary representation of a binary
       index).  Vertices themselves are never > 64 for the range we
       handle (the sequence grows slowly). */
    uint64_t edges[64];
    int ecount = 0;

    seq_t temp = n;
    idx_t pos = 1;          /* 1‑based position of bits in n (LSB). */
    while (temp != (seq_t)0) {
        if (temp & (seq_t)1) {
            /* Binary indices of 'pos'.  Each bit in 'pos' that is 1
               corresponds to a vertex.  The vertex index is the 1‑based
               position of that bit.  */
            uint64_t e = 0;
            uint64_t p_bits = (uint64_t)pos;
            unsigned vpos = 1;
            while (p_bits) {
                if (p_bits & 1ULL) e |= 1ULL << (vpos - 1);
                p_bits >>= 1;
                ++vpos;
            }
            edges[ecount++] = e;
        }
        temp >>= 1;
        ++pos;
    }
    if (ecount == 0) return false; /* no sets */

    /* Collect all vertices that appear in any edge.  We keep a mask
       of used vertices for quick access. */
    uint64_t used_mask = 0;
    for (int i = 0; i < ecount; ++i) used_mask |= edges[i];
    if (used_mask == 0) return false; /* shouldn't happen */

    /* Initially check that the full hypergraph is connected. */
    int parent[64];
    for (int i = 0; i < 64; ++i) parent[i] = i;

    for (int i = 0; i < ecount; ++i) {
        uint64_t e = edges[i];
        int first = -1;
        for (int v = 0; v < 64; ++v) {
            if (e >> v & 1ULL) {
                if (first == -1) first = v;
                else uf_union(first, v, parent);
            }
        }
    }

    bool seen[64] = {0};
    int comps = 0;
    for (int v = 0; v < 64; ++v) {
        if (!(used_mask >> v & 1ULL)) continue;
        int r = uf_find(v, parent);
        if (!seen[r]) {
            seen[r] = true;
            ++comps;
        }
    }
    if (comps != 1) return false; /* not connected */

    /* Test removal of each single edge.  If the remaining system is
       disconnected (or empty), the edge‐connectivity is 1. */
    for (int rem = 0; rem < ecount; ++rem) {
        /* Remaining vertices mask. */
        uint64_t rem_mask = 0;
        for (int i = 0; i < ecount; ++i)
            if (i != rem) rem_mask |= edges[i];

        if (rem_mask == 0) /* empty system after removal */
            return true;

        /* Rebuild union–find on remaining edges. */
        for (int i = 0; i < 64; ++i) parent[i] = i;
        for (int i = 0; i < ecount; ++i) {
            if (i == rem) continue;
            uint64_t e = edges[i];
            int first = -1;
            for (int v = 0; v < 64; ++v) {
                if (e >> v & 1ULL) {
                    if (first == -1) first = v;
                    else uf_union(first, v, parent);
                }
            }
        }

        /* Count components among vertices that remain covered. */
        bool seen2[64] = {0};
        int comps2 = 0;
        for (int v = 0; v < 64; ++v) {
            if (!(rem_mask >> v & 1ULL)) continue;
            int r = uf_find(v, parent);
            if (!seen2[r]) {
                seen2[r] = true;
                ++comps2;
            }
        }
        if (comps2 > 1) return true; /* removal disconnects system */
    }
    return false; /* no single edge is a bridge */
}

/* Public interface for the OEIS sequence.  The function is memoised
   so that repeated calls with arbitrary n are efficient. */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* dynamic array of seq_t */
    static size_t cap   = 0;
    static size_t len   = 0;
    static seq_t next_candidate = (seq_t)1;

    /* Resolve request for the n‑th term (1‑based). */
    while (len < n) {
        /* Find the next integer that satisfies the property. */
        while (!is_special(next_candidate))
            ++next_candidate;

        /* Ensure the cache has room. */
        if (len + 1 > cap) {
            size_t new_cap = cap ? cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(*terms));
            if (!tmp) {
                /* Allocation failed; abort. */
                exit(EXIT_FAILURE);
            }
            terms = tmp;
            cap   = new_cap;
        }

        /* Store the found term. */
        terms[len++] = next_candidate;
        ++next_candidate;
    }

    return terms[n - 1];
}
