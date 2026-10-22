/*
 * A383982  Number of connected subsets of n edges of the cuboctahedron up to
 *          the 48 rotations and reflections of the cuboctahedron.
 * https://oeis.org/A383982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is computed by enumerating all 24‑edge subsets of the *
 *            cuboctahedron, testing connectivity, and grouping connected
 *            subsets * into orbits under the 48 symmetries of the
 *            cuboctahedron. * a(0) = 1, a(n) = 0 for n > 24
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   build_automorphisms, build_cubo_edges, build_cubo_vertices,
 *            build_edge_lookup
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_automorphisms.h"
#include "../utils/build_cubo_edges.h"
#include "../utils/build_cubo_vertices.h"
#include "../utils/build_edge_lookup.h"

/* ------------------------------------------------------------------ */
/* Helper functions to construct the cuboctahedron geometry and its   */
/* symmetry group.                                                     */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Disjoint‑set utilities for connectivity test                       */
/* ------------------------------------------------------------------ */

static inline int dsu_find(int *p, int x) {
    while (p[x] != x) {
        p[x] = p[p[x]];
        x = p[x];
    }
    return x;
}

static bool is_connected(uint32_t mask, const int *edge_u, const int *edge_v) {
    if (mask == 0) return true;        /* empty set is considered connected */

    int parent[12];
    bool used[12] = {0};

    for (int i = 0; i < 12; ++i) parent[i] = i;

    for (int e = 0; e < 24; ++e) if (mask & (1U << e)) {
        int u = edge_u[e];
        int v = edge_v[e];
        used[u] = used[v] = true;
        int a = dsu_find(parent, u);
        int b = dsu_find(parent, v);
        if (a != b) parent[a] = b;
    }

    int root = -1;
    for (int v = 0; v < 12; ++v) if (used[v]) {
        int r = dsu_find(parent, v);
        if (root == -1) root = r;
        else if (root != r) return false;
    }
    return true;
}

static inline uint32_t apply_perm(uint32_t mask, const int *perm) {
    uint32_t res = 0;
    for (int e = 0; e < 24; ++e)
        if (mask & (1U << e))
            res |= 1U << perm[e];
    return res;
}

/* ------------------------------------------------------------------ */
/* Compute all sequence values (0..24)                                  */
/* ------------------------------------------------------------------ */

static void compute_all(seq_t *aVals) {
    static int edge_u[24], edge_v[24];
    static int autom[48][24];
    static bool built = false;
    if (!built) {
        build_cubo_edges(edge_u, edge_v);
        build_automorphisms(autom);
        built = true;
    }

    const size_t totalMasks = ((size_t)1) << 24;
    uint8_t *visited = (uint8_t*)calloc(totalMasks, 1);
    if (!visited) { /* allocation failure, abort */
        exit(1);
    }

    for (uint32_t mask = 0; mask < totalMasks; ++mask) {
        if (visited[mask]) continue;

        if (!is_connected(mask, edge_u, edge_v)) {
            visited[mask] = 1;
            continue;
        }

        int bits = __builtin_popcount(mask);
        aVals[bits] += 1;          /* new orbit representative */

        for (int g = 0; g < 48; ++g) {
            uint32_t m2 = apply_perm(mask, autom[g]);
            visited[m2] = 1;
        }
    }
    free(visited);
}

/* ------------------------------------------------------------------ */
/* Public function                                                         */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n) {
    static seq_t values[25] = {0};
    static bool ready = false;
    if (!ready) {
        compute_all(values);
        ready = true;
    }
    if (n > 24) return (seq_t)0;
    return values[(size_t)n];
}
