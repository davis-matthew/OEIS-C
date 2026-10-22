/*
 * A327374  BII-numbers of set-systems with vertex-connectivity 2.
 * https://oeis.org/A327374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th BII-number whose set‑system has vertex‑connectivity
 *            2 (computed by brute–force search)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool is_connected(uint64_t mask, const uint64_t adj[64]) {
    /* mask contains the set of vertices that exist after removal. */
    if (mask == 0) return false;
    /* find arbitrary start vertex */
    int start = 0;
    while (start < 64 && !(mask & (1ULL << start))) start++;
    if (start == 64) return false;

    uint64_t visited = 0;
    uint64_t frontier = 1ULL << start;
    visited |= frontier;
    while (frontier) {
        uint64_t new_frontier = 0;
        uint64_t f = frontier;
        while (f) {
            int v = __builtin_ctzll(f);
            f &= f - 1;
            uint64_t nb = adj[v] & mask & ~visited;
            new_frontier |= nb;
        }
        frontier = new_frontier;
        visited |= frontier;
    }
    return visited == mask;
}

static bool has_vc2(uint64_t n) {
    /* Build the set‑system derived from n. */
    uint64_t binary_indices[64];
    int bin_cnt = 0;

    /* Binary indices of n (positions of 1 bits). */
    uint64_t temp = n;
    int pos = 0;
    while (temp) {
        if (temp & 1ULL) {
            binary_indices[bin_cnt++] = pos + 1;    /* positions are 1‑based in the definition */
        }
        temp >>= 1;
        pos++;
    }

    /* For each binary index, compute its binary indices again to form edges. */
    uint64_t edges[64];
    int edge_cnt = 0;
    uint64_t all_vertices = 0;      /* union of all vertices in the graph */
    for (int idx = 0; idx < bin_cnt; idx++) {
        uint64_t i = binary_indices[idx];
        uint64_t mask = 0;
        uint64_t t = i;
        int p = 0;
        while (t) {
            if (t & 1ULL) mask |= 1ULL << p;
            t >>= 1;
            p++;
        }
        edges[edge_cnt++] = mask;
        all_vertices |= mask;
    }

    /* Quick reject if too few vertices or already disconnected. */
    if (__builtin_popcountll(all_vertices) < 3) return false;
    uint64_t adj[64] = {0};

    /* Build adjacency from edges. */
    for (int e = 0; e < edge_cnt; e++) {
        uint64_t mask = edges[e];
        /* collect vertices of this edge */
        int verts[64];
        int vcnt = 0;
        uint64_t m = mask;
        while (m) {
            int v = __builtin_ctzll(m);
            verts[vcnt++] = v;
            m &= m - 1;
        }
        for (int u = 0; u < vcnt; u++) {
            for (int w = u + 1; w < vcnt; w++) {
                int a = verts[u];
                int b = verts[w];
                adj[a] |= 1ULL << b;
                adj[b] |= 1ULL << a;
            }
        }
    }

    /* If original graph is disconnected -> vc = 0. */
    if (!is_connected(all_vertices, adj)) return false;

    /* Prepare list of vertex indices present. */
    int verts[64];
    int vcnt = 0;
    for (int v = 0; v < 64; v++) {
        if (all_vertices & (1ULL << v)) verts[vcnt++] = v;
    }

    /* Test single vertex removals. */
    for (int i = 0; i < vcnt; i++) {
        uint64_t mask_rem = all_vertices & ~(1ULL << verts[i]);
        if (mask_rem == 0 || __builtin_popcountll(mask_rem) <= 1) return false; /* disconnects with 1 removal -> vc <=1 */
        if (!is_connected(mask_rem, adj)) return false; /* disconnects with 1 removal -> vc <=1 */
    }

    /* Test pair removals. */
    for (int i = 0; i < vcnt; i++) {
        for (int j = i + 1; j < vcnt; j++) {
            uint64_t mask_rem = all_vertices & ~(1ULL << verts[i]) & ~(1ULL << verts[j]);
            if (mask_rem == 0 || __builtin_popcountll(mask_rem) <= 1) return true; /* leave <=1 vertex -> vc == 2 */
            if (!is_connected(mask_rem, adj)) return true; /* disconnected -> vc == 2 */
        }
    }
    /* No pair disconnects -> vc > 2 */
    return false;
}

seq_t a(idx_t n) {
    idx_t found = 0;
    for (idx_t x = 1; ; x++) {
        if (has_vc2((uint64_t)x)) {
            found++;
            if (found == n) return (seq_t)x;
        }
    }
}
