/*
 * A339731  Let G be the undirected graph with nodes {g_k, k > 0} such that
 *          for any k > 0, g_k is connected to g_{k+1} and g_{A064413(k)} is
 *          connected to g_{A064413(k+1)}; a(n) is the distance between g_1
 *          and g_n.
 * https://oeis.org/A339731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = BFS distance from g_1 to g_n in the graph with edges
 *            k--k+1 and A064413(k)--A064413(k+1); computed by BFS on a
 *            truncated node set 1..2n+100
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9856 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static int *g_dist = NULL;
static long long g_cached = 0;

static void build_graph_dist(long long N)
{
    long long M = 2 * N + 100;          /* node limit */
    long long K = 3 * M;                /* EKG terms generated */
    long long VB = 4 * K + 100;         /* value bound */
    int *e = (int *)calloc((size_t)(K + 2), sizeof(int));
    int *pos = (int *)calloc((size_t)(VB + 2), sizeof(int));
    unsigned char *used = (unsigned char *)calloc((size_t)(VB + 2), 1);
    int *spf = (int *)calloc((size_t)(VB + 2), sizeof(int));
    int *ptr = (int *)calloc((size_t)(VB + 2), sizeof(int));
    long long i, j;
    for (i = 2; i <= VB; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= VB; j += i) if (spf[j] == 0) spf[j] = (int)i;
        }
    }
    e[1] = 1; e[2] = 2;
    used[1] = used[2] = 1;
    pos[1] = 1; pos[2] = 2;
    for (i = 3; i <= K; i++) {
        long long x = e[i - 1];
        long long best = -1;
        while (x > 1) {
            long long p = spf[x];
            while (x % p == 0) x /= p;
            long long c = ptr[p] ? ptr[p] : p;
            while (c <= VB && used[c]) c += p;
            ptr[p] = (int)c;
            if (best < 0 || c < best) best = c;
        }
        e[i] = (int)best;
        used[best] = 1;
        pos[best] = (int)i;
    }
    free(used); free(spf); free(ptr);

    if (g_dist) free(g_dist);
    g_dist = (int *)malloc((size_t)(M + 2) * sizeof(int));
    int *queue = (int *)malloc((size_t)(M + 2) * sizeof(int));
    for (i = 0; i <= M + 1; i++) g_dist[i] = -1;
    long long qh = 0, qt = 0;
    g_dist[1] = 0;
    queue[qt++] = 1;
    while (qh < qt) {
        int v = queue[qh++];
        long long nb[4];
        int cnt = 0;
        nb[cnt++] = (long long)v - 1;
        nb[cnt++] = (long long)v + 1;
        int p = pos[v];
        if (p > 0) {
            if (p - 1 >= 1) nb[cnt++] = e[p - 1];
            if (p + 1 <= K) nb[cnt++] = e[p + 1];
        }
        for (int t = 0; t < cnt; t++) {
            long long u = nb[t];
            if (u >= 1 && u <= M && g_dist[u] < 0) {
                g_dist[u] = g_dist[v] + 1;
                queue[qt++] = (int)u;
            }
        }
    }
    free(queue); free(e); free(pos);
    g_cached = N;
}

seq_t a(idx_t n)
{
    long long nn = (long long)n;
    if (nn < 1) return (seq_t)0;
    if (nn > g_cached || g_dist == NULL) {
        long long N = nn < 1000 ? 1000 : nn;
        build_graph_dist(N);
    }
    return (seq_t)g_dist[nn];
}
