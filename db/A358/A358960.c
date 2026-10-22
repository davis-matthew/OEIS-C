/*
 * A358960  Number of directed Hamiltonian paths of the Platonic graphs (in
 *          the order of tetrahedral, cubical, octahedral, dodecahedral, and
 *          icosahedral graph).
 * https://oeis.org/A358960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of directed Hamiltonian paths in the nth Platonic
 *            graph (n = 1 tetrahedron, 2 cube, 3 octahedron, 4 dodecahedron,
 *            5 icosahedron)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   count_graph, dfs_directed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <float.h>
#include "../utils/count_graph.h"
#include "../utils/dfs_directed.h"

static uint32_t adj_tetra[4];
static uint32_t adj_cube[8];
static uint32_t adj_octa[6];
static uint32_t adj_dodeca[20];
static uint32_t adj_icosa[12];

/* Build the adjacency masks for all five Platonic graphs.
   The geometric constructions are taken from the standard coordinates of
   the Platonic solids.  All computations are done with double precision
   and a simple ε tolerance.  */
static void init_graphs(void)
{
    /* ---- tetrahedron: complete graph K4 ---- */
    for (int i = 0; i < 4; ++i)
        adj_tetra[i] = (~(1u << i)) & ((1u << 4) - 1);

    /* ---- cube: Q3 ---- */
    for (int v = 0; v < 8; ++v) {
        uint32_t mask = 0;
        for (int b = 0; b < 3; ++b)
            mask |= 1u << (v ^ (1 << b));
        adj_cube[v] = mask;
    }

    /* ---- octahedron: vertices on axes; each vertex is adjacent
              to all except itself and its opposite ---- */
    int opp[6] = {5, 4, 3, 2, 1, 0};
    for (int v = 0; v < 6; ++v)
        adj_octa[v] = ((1u << 6) - 1) & (~(1u << v)) & (~(1u << opp[v]));

    /* ---- dodecahedron: build from coordinates ---- */
    double coord_d[20][3];
    int idx = 0;
    int s[2] = {-1, 1};
    /* 8 vertices: (±1,±1,±1) */
    for (int a = 0; a < 2; ++a)
        for (int b = 0; b < 2; ++b)
            for (int c = 0; c < 2; ++c) {
                coord_d[idx][0] = s[a];
                coord_d[idx][1] = s[b];
                coord_d[idx][2] = s[c];
                ++idx;
            }
    /* φ and 1/φ */
    double phi = (1.0 + sqrt(5.0)) / 2.0;
    double invphi = 1.0 / phi;
    /* 12 vertices: permutations of (0, ±1/φ, ±φ) */
    for (int a = 0; a < 2; ++a)
        for (int b = 0; b < 2; ++b) {
            coord_d[idx][0] = 0.0;
            coord_d[idx][1] = s[a] * invphi;
            coord_d[idx][2] = s[b] * phi;
            ++idx;
        }
    for (int a = 0; a < 2; ++a)
        for (int b = 0; b < 2; ++b) {
            coord_d[idx][0] = s[a] * invphi;
            coord_d[idx][1] = s[b] * phi;
            coord_d[idx][2] = 0.0;
            ++idx;
        }
    for (int a = 0; a < 2; ++a)
        for (int b = 0; b < 2; ++b) {
            coord_d[idx][0] = s[a] * phi;
            coord_d[idx][1] = 0.0;
            coord_d[idx][2] = s[b] * invphi;
            ++idx;
        }
    /* find minimal non-zero squared distance */
    double min_d2 = DBL_MAX;
    for (int i = 0; i < 20; ++i)
        for (int j = i + 1; j < 20; ++j) {
            double dx = coord_d[i][0] - coord_d[j][0];
            double dy = coord_d[i][1] - coord_d[j][1];
            double dz = coord_d[i][2] - coord_d[j][2];
            double d2 = dx*dx + dy*dy + dz*dz;
            if (d2 > 0.0 && d2 < min_d2)
                min_d2 = d2;
        }
    double eps = 1e-6;
    for (int i = 0; i < 20; ++i) {
        uint32_t mask = 0;
        for (int j = 0; j < 20; ++j) if (i != j) {
            double dx = coord_d[i][0] - coord_d[j][0];
            double dy = coord_d[i][1] - coord_d[j][1];
            double dz = coord_d[i][2] - coord_d[j][2];
            double d2 = dx*dx + dy*dy + dz*dz;
            if (fabs(d2 - min_d2) <= eps * min_d2)
                mask |= 1u << j;
        }
        adj_dodeca[i] = mask;
    }

    /* ---- icosahedron: build from coordinates ---- */
    double coord_i[12][3];
    idx = 0;
    /* 12 vertices: permutations of (0, ±1, ±φ) */
    for (int a = 0; a < 2; ++a)
        for (int b = 0; b < 2; ++b) {
            coord_i[idx][0] = 0.0;
            coord_i[idx][1] = s[a] * 1.0;
            coord_i[idx][2] = s[b] * phi;
            ++idx;
        }
    for (int a = 0; a < 2; ++a)
        for (int b = 0; b < 2; ++b) {
            coord_i[idx][0] = s[a] * 1.0;
            coord_i[idx][1] = s[b] * phi;
            coord_i[idx][2] = 0.0;
            ++idx;
        }
    for (int a = 0; a < 2; ++a)
        for (int b = 0; b < 2; ++b) {
            coord_i[idx][0] = s[a] * phi;
            coord_i[idx][1] = 0.0;
            coord_i[idx][2] = s[b] * 1.0;
            ++idx;
        }
    /* minimal non‑zero squared distance */
    min_d2 = DBL_MAX;
    for (int i = 0; i < 12; ++i)
        for (int j = i + 1; j < 12; ++j) {
            double dx = coord_i[i][0] - coord_i[j][0];
            double dy = coord_i[i][1] - coord_i[j][1];
            double dz = coord_i[i][2] - coord_i[j][2];
            double d2 = dx*dx + dy*dy + dz*dz;
            if (d2 > 0.0 && d2 < min_d2)
                min_d2 = d2;
        }
    for (int i = 0; i < 12; ++i) {
        uint32_t mask = 0;
        for (int j = 0; j < 12; ++j) if (i != j) {
            double dx = coord_i[i][0] - coord_i[j][0];
            double dy = coord_i[i][1] - coord_i[j][1];
            double dz = coord_i[i][2] - coord_i[j][2];
            double d2 = dx*dx + dy*dy + dz*dz;
            if (fabs(d2 - min_d2) <= eps * min_d2)
                mask |= 1u << j;
        }
        adj_icosa[i] = mask;
    }
}

seq_t a(idx_t n)
{
    static seq_t cache[6] = {0};   /* indices 1..5 */
    static bool initialized = false;

    if (!initialized) {
        init_graphs();
        initialized = true;
    }

    if (n == 0 || n > 5)
        return 0;   /* sequence not defined beyond n = 5 */

    if (cache[n] != 0)
        return cache[n];

    const uint32_t *adj;
    int nverts;
    switch ((idx_t)n) {
        case 1: adj = adj_tetra;  nverts = 4;  break;
        case 2: adj = adj_cube;   nverts = 8;  break;
        case 3: adj = adj_octa;   nverts = 6;  break;
        case 4: adj = adj_dodeca; nverts = 20; break;
        case 5: adj = adj_icosa;  nverts = 12; break;
        default: return 0;
    }

    cache[n] = count_graph(nverts, adj);
    return cache[n];
}
