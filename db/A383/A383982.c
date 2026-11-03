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
 * Validated: full, pass, 25 terms through n = 24 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static void build_cubo_vertices(int coords[12][3]) {
    int idx = 0;
    for (int zero = 0; zero < 3; ++zero) {
        for (int s1 = -1; s1 <= 1; s1 += 2) {
            for (int s2 = -1; s2 <= 1; s2 += 2) {
                int c[3] = {0, 0, 0};
                c[zero]          = 0;
                c[(zero + 1) % 3] = s1;
                c[(zero + 2) % 3] = s2;
                for (int i = 0; i < 3; ++i) coords[idx][i] = c[i];
                ++idx;
            }
        }
    }
}

static void build_cubo_edges(int edge_u[24], int edge_v[24]) {
    int coords[12][3];
    build_cubo_vertices(coords);
    int edge_cnt = 0;
    for (int i = 0; i < 12; ++i) {
        for (int j = i + 1; j < 12; ++j) {
            int dx = coords[i][0] - coords[j][0];
            int dy = coords[i][1] - coords[j][1];
            int dz = coords[i][2] - coords[j][2];
            int d2 = dx * dx + dy * dy + dz * dz;
            if (d2 == 2) {          /* distance sqrt(2) → edge */
                edge_u[edge_cnt] = i;
                edge_v[edge_cnt] = j;
                ++edge_cnt;
            }
        }
    }
}

static void build_edge_lookup(const int edge_u[24], const int edge_v[24],
                              int lookup[144]) {
    for (int i = 0; i < 144; ++i) lookup[i] = -1;
    for (int e = 0; e < 24; ++e) {
        int a = edge_u[e], b = edge_v[e];
        if (a > b) { int t = a; a = b; b = t; }
        lookup[a * 12 + b] = e;
    }
}

static void build_automorphisms(int autom[48][24]) {
    const int perms[6][3] = {
        {0,1,2},{0,2,1},{1,0,2},
        {1,2,0},{2,0,1},{2,1,0}
    };
    const int signs[8][3] = {
        { 1, 1, 1},{ 1, 1,-1},{ 1,-1, 1},{ 1,-1,-1},
        {-1, 1, 1},{-1, 1,-1},{-1,-1, 1},{-1,-1,-1}
    };
    int coords[12][3];
    build_cubo_vertices(coords);
    int edge_u[24], edge_v[24];
    build_cubo_edges(edge_u, edge_v);
    int lookup[144];
    build_edge_lookup(edge_u, edge_v, lookup);

    int perm_idx = 0;
    for (int p = 0; p < 6; ++p) {
        const int *axis = perms[p];
        for (int s = 0; s < 8; ++s) {
            const int *sgn = signs[s];
            int vertex_map[12];
            for (int v = 0; v < 12; ++v) {
                int nv[3];
                for (int d = 0; d < 3; ++d)
                    nv[d] = sgn[d] * coords[v][axis[d]];
                int found = -1;
                for (int u = 0; u < 12; ++u)
                    if (nv[0] == coords[u][0] &&
                        nv[1] == coords[u][1] &&
                        nv[2] == coords[u][2]) { found = u; break; }
                vertex_map[v] = found;
            }
            for (int e = 0; e < 24; ++e) {
                int u = vertex_map[edge_u[e]];
                int v = vertex_map[edge_v[e]];
                if (u > v) { int t = u; u = v; v = t; }
                int idx = lookup[u * 12 + v];
                autom[perm_idx][e] = idx;
            }
            ++perm_idx;
        }
    }
}

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
