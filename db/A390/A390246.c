/*
 * A390246  a(n) is the least integer k such that there exist n distinct
 *          integers b_1, ..., b_n with n < b_i < n+k and b_i is divisible by
 *          i for 1 <= i <= n.
 * https://oeis.org/A390246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) is computed by binary searching minimal k such that a
 *            bipartite matching exists between the indices 1..n (left side)
 *            and the integers n+1..n+k-1 (right side), where an edge (i, v)
 *            exists iff v is divisible by i
 * Range:     u8 n <= 155, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9415 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Required utilities are not used.  All operations are performed with
    idx_t for indices and seq_t only for the result.               */

#include <stdlib.h>
#include <stdbool.h>

/* Depth‑first search for augmenting paths used by Hopcroft–Karp. */
static bool dfs_uv(int u, const int *start, const int *edges,
                   int *pairU, int *pairV, int *distU, int n)
{
    if (u == 0) return true;
    int s = start[u], e = start[u + 1];
    for (int ei = s; ei < e; ++ei) {
        int v = edges[ei];
        int pu = pairV[v];
        if (pu == 0 || (distU[pu] == distU[u] + 1 && dfs_uv(pu, start, edges,
                                                             pairU, pairV, distU, n))) {
            pairU[u] = v;
            pairV[v] = u;
            return true;
        }
    }
    distU[u] = -1;    /* prune this vertex for remaining DFS calls */
    return false;
}

/* Test whether for given n,k a perfect matching exists. */
static bool has_matching(idx_t n, idx_t k)
{
    if (k <= 1) return false;          /* interval would be empty */
    size_t m = (size_t)(k - 1);        /* number of right vertices */

    /* --- Build adjacency --- */
    int *start = (int*)malloc((size_t)(n + 2) * sizeof(int));
    int *edgeCnt = (int*)calloc((size_t)(n + 1), sizeof(int));

    /* Count edges first */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t first = (n / i + 1) * i;
        if (first > n + k - 1) continue;
        for (idx_t val = first; val <= n + k - 1; val += i)
            ++edgeCnt[(int)i];
    }

    size_t totalEdges = 0;
    for (idx_t i = 1; i <= n; ++i) {
        start[i] = (int)totalEdges;
        totalEdges += (size_t)edgeCnt[(int)i];
    }
    start[n + 1] = (int)totalEdges;

    int *edges = (int*)malloc(totalEdges * sizeof(int));
    int *current = (int*)calloc((size_t)(n + 1), sizeof(int));

    for (idx_t i = 1; i <= n; ++i) {
        idx_t first = (n / i + 1) * i;
        if (first > n + k - 1) continue;
        for (idx_t val = first; val <= n + k - 1; val += i) {
            int v = (int)(val - (n + 1)); /* right vertex index 0‑based */
            int pos = start[(int)i] + current[(int)i];
            edges[pos] = v + 1;          /* store 1‑based for matching */
            ++current[(int)i];
        }
    }
    free(edgeCnt);
    free(current);

    /* --- Matching arrays --- */
    int *pairU = (int*)calloc((size_t)(n + 1), sizeof(int));
    int *pairV = (int*)calloc((size_t)(m + 1), sizeof(int));
    int *distU  = (int*)calloc((size_t)(n + 1), sizeof(int));

    int matching = 0;
    while (true) {
        /* BFS: build layers */
        int *queue = (int*)malloc((size_t)(n + 1) * sizeof(int));
        int qh = 0, qt = 0;
        for (int u = 1; u <= n; ++u) {
            if (pairU[u] == 0) {
                distU[u] = 0;
                queue[qt++] = u;
            } else
                distU[u] = -1;
        }
        bool foundFree = false;
        while (qh < qt) {
            int u = queue[qh++];
            int s = start[u], e = start[u + 1];
            for (int ei = s; ei < e; ++ei) {
                int v = edges[ei];
                if (pairV[v] == 0) {
                    foundFree = true;
                } else {
                    int pu = pairV[v];
                    if (distU[pu] == -1) {
                        distU[pu] = distU[u] + 1;
                        queue[qt++] = pu;
                    }
                }
            }
        }
        free(queue);
        if (!foundFree) break;          /* no augmenting path */

        /* DFS stage */
        for (int u = 1; u <= n; ++u) {
            if (pairU[u] == 0 && dfs_uv(u, start, edges, pairU,
                                         pairV, distU, (int)n))
                ++matching;
        }
    }

    /* Clean up */
    free(start);
    free(edges);
    free(pairU);
    free(pairV);
    free(distU);

    return (matching == (int)n);
}

/* Forward declaration of helper */
static bool has_matching(idx_t n, idx_t k);

/* Main entry function required by the judge. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not defined, but safe */
    idx_t low = 2;                  /* smallest possible k */
    idx_t high = 3 * n;             /* safe upper bound (k ≤ 3n) */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (has_matching(n, mid))
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
