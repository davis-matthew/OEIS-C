/*
 * A327376  BII-numbers of set-systems with vertex-connectivity 3.
 * https://oeis.org/A327376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the BII‑numbers of set‑systems with vertex‑connectivity 3
   by brute‑forcing every integer and testing its derived set‑system.
   a(1) is the first such integer, a(2) the second, etc. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Helper: collect 1‑bit positions (1‑based) of n into buf[0..63].
   *cnt receives number of positions.                          */
static void indices_of(idx_t n, uint8_t *buf, int *cnt)
{
    int k = 0;
    int pos = 1;
    while (n) {
        if (n & 1ULL)
            buf[k++] = (uint8_t)pos;
        n >>= 1;
        ++pos;
    }
    *cnt = k;
}

/* Connectivity test on a vertex set described by a bitmask.
   The adjacency graph is given by adj[i] (bit i of adj[i] is set
   iff vertex i is adjacent to vertex j, where i and j are 0‑based). */
static bool is_connected_mask(const uint64_t adj[64], uint64_t mask)
{
    if ((mask & (mask - 1)) == 0)   /* 0 or 1 vertex */
        return false;
    /* find first vertex in mask */
    int start = __builtin_ctzll(mask);
    uint64_t frontier = 1ULL << start;
    uint64_t visited  = frontier;
    while (frontier) {
        uint64_t v = frontier & -frontier;      /* least significant set bit */
        frontier ^= v;
        int vi = __builtin_ctzll(v);
        uint64_t neigh = adj[vi] & mask;
        uint64_t new_frontier = neigh & ~visited;
        frontier |= new_frontier;
        visited  |= new_frontier;
    }
    return visited == mask;
}

/* Test whether the set‑system derived from n has vertex‑connectivity 3. */
static bool has_vertex_connectivity_3(idx_t n)
{
    if (n == 0)
        return false;

    /* Size limits: vertex labels are between 1 and 64 (bits). */
    bool present[65] = {0};             /* present[label] */
    uint64_t adj[64] = {0};             /* adjacency bitmasks */
    uint8_t   derived[64];
    int       dcnt;
    uint8_t  inner[64];
    int       icnt;
    int       verts[64];
    int       V = 0;

    /* First pass: build graph. */
    for (idx_t pos = 1, bit = n; bit; ++pos, bit >>= 1) {
        if (bit & 1ULL) {
            /* binary indices of pos */
            indices_of(pos, inner, &icnt);
            /* add vertices and edges */
            for (int a = 0; a < icnt; ++a) {
                int va = inner[a] - 1;   /* 0‑based index of vertex */
                present[inner[a]] = true;
                for (int b = a + 1; b < icnt; ++b) {
                    int vb = inner[b] - 1;
                    adj[va] |= 1ULL << vb;
                    adj[vb] |= 1ULL << va;
                }
            }
        }
    }

    /* Count vertices. */
    for (int lbl = 1; lbl <= 64; ++lbl)
        if (present[lbl])
            verts[V++] = lbl - 1;   /* store 0‑based index */

    if (V < 4)   /* need at least 4 vertices for κ = 3 */
        return false;

    uint64_t full_mask = 0;
    for (int i = 0; i < V; ++i)
        full_mask |= 1ULL << verts[i];

    /* Full graph must be connected. */
    if (!is_connected_mask(adj, full_mask))
        return false;

    /* Removing any 1 or 2 vertices must still keep connectivity. */
    /* 1‑vertex removals. */
    for (int i = 0; i < V; ++i) {
        uint64_t mask = full_mask & ~(1ULL << verts[i]);
        if (!is_connected_mask(adj, mask))
            return false;
    }
    /* 2‑vertex removals. */
    for (int i = 0; i < V; ++i)
        for (int j = i + 1; j < V; ++j) {
            uint64_t mask = full_mask & ~(1ULL << verts[i]) & ~(1ULL << verts[j]);
            if (!is_connected_mask(adj, mask))
                return false;
        }

    /* There must be at least one 3‑vertex removal that disconnects. */
    bool found = false;
    for (int i = 0; i < V && !found; ++i)
        for (int j = i + 1; j < V && !found; ++j)
            for (int k = j + 1; k < V; ++k) {
                uint64_t mask = full_mask
                              & ~(1ULL << verts[i])
                              & ~(1ULL << verts[j])
                              & ~(1ULL << verts[k]);
                if (!is_connected_mask(adj, mask)) {
                    found = true;
                    break;
                }
            }

    return found;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Cache of results for fast O(1) lookup after initial computation. */
static seq_t *cache        = NULL;
static idx_t  cache_len    = 0;
static idx_t  cache_cap    = 0;
static idx_t  next_candidate = 1;  /* next integer to test */

static void ensure_cache(idx_t need)
{
    if (cache_len >= need)
        return;

    if (cache_cap < need) {
        cache_cap = (cache_cap > 0) ? cache_cap * 2 : 128;
        while (cache_cap < need)
            cache_cap <<= 1;
        cache = mem_resize(seq_t, cache, cache_cap);
    }

    while (cache_len < need) {
        if (has_vertex_connectivity_3(next_candidate))
            cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)         /* Undefined below offset; return 0 */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
