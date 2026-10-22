/*
 * A327082  BII-numbers of set-systems with cut-connectivity 2.
 * https://oeis.org/A327082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A327082: BII‑numbers of set‑systems with cut‑connectivity 2.
   The function generates the BII‑number of the nth set‑system in that
   property by brute‑force search.  The search enumerates binary indices,
   constructs the corresponding hypergraph, checks its cut‑connectivity,
   and stores all suitable numbers in a cache to answer arbitrary
   queries efficiently.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* DSU helper: find with path compression (indices 0..63). */
static inline int dsu_find(int x, int parent[]) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

/* Returns true if removing the vertices indicated by removal_mask
   disconnects the hypergraph described by edge_masks. */
static bool has_cut_mask(uint64_t removal_mask,
                        const uint64_t edge_masks[64], int edgeCnt,
                        uint64_t vertex_mask)
{
    int parent[64];
    for (int i = 0; i < 64; ++i) parent[i] = i;

    bool hasEdge = false;
    uint64_t newVMask = 0;

    for (int e = 0; e < edgeCnt; ++e) {
        uint64_t nm = edge_masks[e] & ~removal_mask;
        if (nm == 0) continue;
        hasEdge = true;
        newVMask |= nm;

        int first = __builtin_ctzll(nm);
        uint64_t t = nm & (nm - 1);
        while (t) {
            int nxt = __builtin_ctzll(t);
            int r1 = dsu_find(first, parent);
            int r2 = dsu_find(nxt, parent);
            if (r1 != r2) parent[r2] = r1;
            t &= t - 1;
        }
    }

    if (!hasEdge) return true;          /* empty hypergraph */

    bool seen[64] = {0};
    int comps = 0;
    for (int i = 0; i < 64; ++i) {
        if (newVMask & (1ULL << i)) {
            int r = dsu_find(i, parent);
            if (!seen[r]) { seen[r] = 1; ++comps; }
        }
    }
    return comps > 1;                   /* disconnected after removal */
}

/* Checks whether the BII‑number n corresponds to a set‑system
   with cut‑connectivity exactly 2. */
static bool is_connectivity_two(idx_t n)
{
    uint64_t edge_masks[64];
    int edgeCnt = 0;
    uint64_t vertexMask = 0;

    uint64_t tmp = n;
    while (tmp) {
        int bit = __builtin_ctzll(tmp);          /* position of set bit (0‑based) */
        int p = bit + 1;                         /* actual value of the index */

        /* compute binary indices of p */
        uint64_t mask = 0;
        uint64_t qp = p;
        while (qp) {
            int qbit = __builtin_ctzll(qp);
            mask |= 1ULL << qbit;                /* vertex label = qbit+1 */
            qp &= qp - 1;
        }
        edge_masks[edgeCnt++] = mask;
        vertexMask |= mask;
        tmp &= tmp - 1;
    }

    /* If already disconnected, skip. */
    if (has_cut_mask(0, edge_masks, edgeCnt, vertexMask))
        return false;

    /* List all present vertices (0‑based). */
    int verts[64], numVerts = 0;
    uint64_t vt = vertexMask;
    while (vt) {
        int v = __builtin_ctzll(vt);
        verts[numVerts++] = v;
        vt &= vt - 1;
    }

    /* Check for a singleton cut (cut‑connectivity = 1). */
    for (int a = 0; a < numVerts; ++a) {
        uint64_t rem = 1ULL << verts[a];
        if (has_cut_mask(rem, edge_masks, edgeCnt, vertexMask))
            return false;
    }

    /* Check for a pair cut (cut‑connectivity = 2). */
    for (int a = 0; a < numVerts; ++a) {
        for (int b = a + 1; b < numVerts; ++b) {
            uint64_t rem = (1ULL << verts[a]) | (1ULL << verts[b]);
            if (has_cut_mask(rem, edge_masks, edgeCnt, vertexMask))
                return true;
        }
    }

    return false;   /* cut‑connectivity > 2 or 0 */
}

/* Cache for generated terms. */
static seq_t *cache = NULL;
static size_t cache_size = 0, cache_cap = 0;
static idx_t next_candidate = 1;

/* Returns the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    while (cache_size < (size_t)n) {
        if (is_connectivity_two(next_candidate)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
