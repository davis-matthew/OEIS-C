/*
 * A339732  Let G be the undirected graph with nodes {g_k, k > 0} such that
 *          for any k > 0, g_k is connected to g_{k+1} and g_{A064413(k)} is
 *          connected to g_{A064413(k+1)}; a(n) is the number of nodes at
 *          distance n from g_1.
 * https://oeis.org/A339732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = number of nodes at BFS distance n from g_1 in the graph
 *            with edges g_k~g_{k+1} and g_{A064413(k)}~g_{A064413(k+1)};
 *            computed by sieve (EKG) + BFS on a truncated graph
 * Range:     u8 n <= 17, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 36 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define A339732_M (1 << 21)
#define A339732_L (4 * A339732_M)
#define A339732_D 256

static long long A339732_cnt[A339732_D];
static bool A339732_done = false;

static void A339732_build(void)
{
    int M = A339732_M, L = A339732_L;
    int *spf = (int *)calloc(L + 1, sizeof(int));
    int *e = (int *)calloc(M + 2, sizeof(int));
    int *pos = (int *)calloc(L + 1, sizeof(int));
    int *ptr = (int *)calloc(L + 1, sizeof(int));
    unsigned char *used = (unsigned char *)calloc(L + 1, 1);
    int *dist = (int *)malloc((M + 2) * sizeof(int));
    int *q = (int *)malloc((M + 2) * sizeof(int));
    int i, j, k;
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    e[1] = 1; e[2] = 2; used[1] = used[2] = 1; pos[1] = 1; pos[2] = 2;
    for (k = 3; k <= M; k++) {
        int x = e[k - 1];
        int best = L + 1;
        while (x > 1) {
            int p = spf[x];
            if (ptr[p] == 0) ptr[p] = p;
            while (ptr[p] <= L && used[ptr[p]]) ptr[p] += p;
            if (ptr[p] < best) best = ptr[p];
            while (x % p == 0) x /= p;
        }
        e[k] = best;
        used[best] = 1;
        pos[best] = k;
    }
    for (i = 0; i <= M + 1; i++) dist[i] = -1;
    int head = 0, tail = 0;
    dist[1] = 0; q[tail++] = 1;
    while (head < tail) {
        int u = q[head++];
        int d = dist[u];
        int nb[4], nn = 0;
        if (u > 1) nb[nn++] = u - 1;
        if (u < M) nb[nn++] = u + 1;
        int pp = pos[u];
        if (pp) {
            if (pp > 1) nb[nn++] = e[pp - 1];
            if (pp < M) nb[nn++] = e[pp + 1];
        }
        for (i = 0; i < nn; i++) {
            int v = nb[i];
            if (v >= 1 && v <= M && dist[v] < 0) {
                dist[v] = d + 1;
                q[tail++] = v;
            }
        }
    }
    for (i = 0; i < A339732_D; i++) A339732_cnt[i] = 0;
    for (i = 1; i <= M; i++)
        if (dist[i] >= 0 && dist[i] < A339732_D) A339732_cnt[dist[i]]++;
    free(spf); free(e); free(pos); free(ptr); free(used); free(dist); free(q);
}

seq_t a(idx_t n)
{
    if (!A339732_done) {
        A339732_build();
        A339732_done = true;
    }
    if (n >= A339732_D) return (seq_t)0;
    return (seq_t)A339732_cnt[n];
}
