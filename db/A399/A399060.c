/*
 * A399060  a(n) is the maximum size of a subset of the distinct entries in
 *          row n of Pascal's triangle such that no selected entry divides
 *          another.
 * https://oeis.org/A399060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = width of the divisibility poset of distinct entries of
 *            row n of Pascal's triangle. The width equals |P_n| minus the
 *            size of a maximum matching in a bipartite graph formed from the
 *            poset. The implementation uses the explicit prime‑exponent
 *            representation of binomial coefficients to test com...
 * Range:     u8 n <= 560, u16 n <= 6068, u32 n <= 6068, u64 n <= 6068,
 *            u128 n <= 6068, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/build_primes.h"

typedef struct {
    int *data;
    int len;
    int cap;
} vec_t;

static inline void vec_init(vec_t *v, int cap) {
    v->cap = cap;
    v->len = 0;
    v->data = (int *)malloc(cap * sizeof(int));
}
static inline void vec_push(vec_t *v, int val) {
    if (v->len == v->cap) {
        v->cap <<= 1;
        v->data = (int *)realloc(v->data, v->cap * sizeof(int));
    }
    v->data[v->len++] = val;
}
static inline void vec_free(vec_t *v) {
    free(v->data);
}

static int hk_bfs(int *pairU, int *pairV, int *dist, const vec_t *adj, int n) {
    int *queue = (int *)malloc(n * sizeof(int));
    int head = 0, tail = 0;
    const int INF = INT_MAX;
    for (int u = 0; u < n; ++u) {
        if (pairU[u] == -1) {
            dist[u] = 0;
            queue[tail++] = u;
        } else {
            dist[u] = INF;
        }
    }
    int reachable = 0;
    while (head < tail) {
        int u = queue[head++];
        for (int k = 0; k < adj[u].len; ++k) {
            int v = adj[u].data[k];
            int pu = pairV[v];
            if (pu == -1) {
                reachable = 1;
            } else if (dist[pu] == INF) {
                dist[pu] = dist[u] + 1;
                queue[tail++] = pu;
            }
        }
    }
    free(queue);
    return reachable;
}

static bool hk_dfs(int u, int *pairU, int *pairV, int *dist, const vec_t *adj) {
    for (int k = 0; k < adj[u].len; ++k) {
        int v = adj[u].data[k];
        int pu = pairV[v];
        if (pu == -1 || (dist[pu] == dist[u] + 1 && hk_dfs(pu, pairU, pairV, dist, adj))) {
            pairU[u] = v;
            pairV[v] = u;
            return true;
        }
    }
    dist[u] = INT_MAX;
    return false;
}

static int hk_max_matching(const vec_t *adj, int n) {
    int *pairU = (int *)calloc(n, sizeof(int));
    int *pairV = (int *)calloc(n, sizeof(int));
    int *dist  = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) pairU[i] = pairV[i] = -1;
    int matching = 0;
    while (hk_bfs(pairU, pairV, dist, adj, n)) {
        for (int u = 0; u < n; ++u) {
            if (pairU[u] == -1 && hk_dfs(u, pairU, pairV, dist, adj))
                ++matching;
        }
    }
    free(pairU);
    free(pairV);
    free(dist);
    return matching;
}

seq_t a(idx_t n) {
    /* number of distinct entries in row n (0<=k<=floor(n/2)) */
    int m = (int)(n / 2 + 1);          /* 0 <= m <= 5000 for n <= 10000 */

    /* trivial small n */
    if (n < 4 || m <= 1) return (seq_t)1;

    /* primes up to n */
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes(n, (idx_t **)&primes, &prime_cnt);

    if (prime_cnt == 0) {
        free(primes);
        return (seq_t)1;
    }

    /* vp[p][i] = exponent of primes[p] in i!   (i = 0..n) */
    size_t vp_size = prime_cnt * (size_t)(n + 1);
    uint16_t *vp = (uint16_t *)malloc(vp_size * sizeof(uint16_t));
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];
        uint16_t *arr = vp + pi * (n + 1);
        arr[0] = 0;
        for (uint64_t i = 1; i <= n; ++i) {
            uint64_t t = i;
            uint16_t cnt = 0;
            while (t % p == 0) {
                ++cnt;
                t /= p;
            }
            arr[i] = arr[i - 1] + cnt;
        }
    }

    /* e[p][k] = exponent of primes[p] in C(n,k), 0<=k<m */
    size_t e_size = prime_cnt * (size_t)m;
    uint16_t *e = (uint16_t *)malloc(e_size * sizeof(uint16_t));
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint16_t *arr = vp + pi * (n + 1);
        uint16_t Vn = arr[n];
        uint16_t *dest = e + pi * m;
        for (int k = 0; k < m; ++k) {
            dest[k] = Vn - arr[k] - arr[n - k];
        }
    }

    /* adjacency lists for comparability graph (U -> V) */
    vec_t *adj = (vec_t *)malloc(m * sizeof(vec_t));
    for (int i = 0; i < m; ++i) vec_init(&adj[i], 4);

    for (int i = 0; i < m; ++i) {
        for (int j = i + 1; j < m; ++j) {
            bool ok = true;
            for (size_t pi = 0; pi < prime_cnt; ++pi) {
                if (e[pi * m + i] > e[pi * m + j]) {
                    ok = false;
                    break;
                }
            }
            if (ok) vec_push(&adj[i], j);
        }
    }

    int matching = hk_max_matching(adj, m);
    int width = m - matching;

    /* free memory */
    for (int i = 0; i < m; ++i) vec_free(&adj[i]);
    free(adj);
    free(e);
    free(vp);
    free(primes);

    return (seq_t)width;
}
