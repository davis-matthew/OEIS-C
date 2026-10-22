/*
 * A329770  Coordination sequence for trivalent vertex in 1-skeleton of
 *          triakis icosahedron.
 * https://oeis.org/A329770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence for trivalent vertex in 1-skeleton of triakis icosahedron
   computed by building the graph of the polyhedron and counting vertices at
   graph distance n from a chosen trivalent vertex. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* static adjacency data, built once */
    static bool built = false;
    static int neigh[32][10];
    static int neigh_cnt[32];

    if (!built)
    {
        /* 12 base vertices of the icosahedron */
        const double phi = (1.0 + sqrt(5.0)) / 2.0;
        double coord[12][3] = {
            {0, 1,  phi}, {0, -1, phi},
            {0, 1, -phi}, {0, -1, -phi},
            {1,  phi, 0}, {-1,  phi, 0},
            {1, -phi, 0}, {-1, -phi, 0},
            {phi, 0,  1}, {phi, 0, -1},
            {-phi, 0, 1}, {-phi, 0, -1}
        };

        bool adj_base[12][12] = {0};
        /* detect edges between base vertices (edge length squared = 4) */
        const double tol = 1e-6;
        for (int i = 0; i < 12; ++i) {
            for (int j = i + 1; j < 12; ++j) {
                double dx = coord[i][0] - coord[j][0];
                double dy = coord[i][1] - coord[j][1];
                double dz = coord[i][2] - coord[j][2];
                double d2 = dx * dx + dy * dy + dz * dz;
                if (fabs(d2 - 4.0) <= tol)
                    adj_base[i][j] = adj_base[j][i] = true;
            }
        }

        /* list all faces (triangles) of the icosahedron */
        int face[20][3];
        int face_cnt = 0;
        for (int i = 0; i < 12; ++i)
            for (int j = i + 1; j < 12; ++j)
                for (int k = j + 1; k < 12; ++k)
                    if (adj_base[i][j] && adj_base[j][k] && adj_base[i][k]) {
                        face[face_cnt][0] = i;
                        face[face_cnt][1] = j;
                        face[face_cnt][2] = k;
                        ++face_cnt;
                    }

        /* initialize neighbor counts */
        for (int v = 0; v < 32; ++v)
            neigh_cnt[v] = 0;

        /* base vertices: connect to other base vertices */
        for (int v = 0; v < 12; ++v) {
            for (int u = 0; u < 12; ++u)
                if (adj_base[v][u])
                    neigh[v][neigh_cnt[v]++] = u;
        }

        /* apices: faces 12..31, each connected to its 3 base vertices */
        for (int f = 0; f < face_cnt; ++f) {
            int apex = 12 + f;
            for (int k = 0; k < 3; ++k)
                neigh[apex][k] = face[f][k];
            neigh_cnt[apex] = 3;
            for (int k = 0; k < 3; ++k) {
                int b = face[f][k];
                neigh[b][neigh_cnt[b]++] = apex;
            }
        }

        built = true;
    }

    /* BFS from the first apex vertex (index 12) */
    int queue[32];
    bool visited[32] = {0};
    idx_t dist[32] = {0};

    int head = 0, tail = 0;
    int start = 12;
    visited[start] = true;
    dist[start] = 0;
    queue[tail++] = start;

    while (head < tail) {
        int cur = queue[head++];
        idx_t dnext = dist[cur] + 1;
        for (int k = 0; k < neigh_cnt[cur]; ++k) {
            int nxt = neigh[cur][k];
            if (!visited[nxt]) {
                visited[nxt] = true;
                dist[nxt] = dnext;
                queue[tail++] = nxt;
            }
        }
    }

    /* count vertices at distance n */
    seq_t count = 0;
    for (int v = 0; v < 32; ++v) {
        if (dist[v] == n)
            ++count;
    }
    return count;
}
