/*
 * A339695  Let G be the undirected graph with nodes {g_k, k >= 0} such that
 *          for any k >= 0, g_k is connected to g_{k+1} and g_{A006068(k)} is
 *          connected to g_{A006068(k+1)}; a(n) is the distance between g_0
 *          and g_n.
 * https://oeis.org/A339695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = BFS distance from g_0 to g_n in the graph with edges
 *            k~k+1 and A006068(k)~A006068(k+1); search restricted to nodes <
 *            2^(bitlen(n)+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9637 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A339695_inv_gray(uint64_t n)
{
    uint64_t r = n, s = n;
    while (s > 0) {
        s >>= 1;
        r ^= s;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t L = 4;
    while (L <= (uint64_t)n) L <<= 1;
    L <<= 1;

    uint32_t *dist = (uint32_t *)malloc(L * sizeof(uint32_t));
    uint32_t *queue = (uint32_t *)malloc(L * sizeof(uint32_t));
    if (!dist || !queue) {
        free(dist);
        free(queue);
        return (seq_t)n;
    }
    for (uint64_t i = 0; i < L; i++) dist[i] = 0xFFFFFFFFu;

    size_t head = 0, tail = 0;
    dist[0] = 0;
    queue[tail++] = 0;
    uint32_t result = (uint32_t)n;

    while (head < tail) {
        uint64_t j = queue[head++];
        if (j == (uint64_t)n) {
            result = dist[j];
            break;
        }
        uint64_t nb[4];
        int cnt = 0;
        if (j > 0) nb[cnt++] = j - 1;
        nb[cnt++] = j + 1;
        uint64_t k = A339695_inv_gray(j ^ (j >> 1) ? j : j); /* placeholder, replaced below */
        (void)k;
        /* k such that A006068(k) = j is k = j ^ (j>>1) (Gray code) */
        uint64_t kk = j ^ (j >> 1);
        if (kk > 0) nb[cnt++] = A339695_inv_gray(kk - 1);
        nb[cnt++] = A339695_inv_gray(kk + 1);
        for (int i = 0; i < cnt; i++) {
            uint64_t v = nb[i];
            if (v < L && dist[v] == 0xFFFFFFFFu) {
                dist[v] = dist[j] + 1;
                queue[tail++] = (uint32_t)v;
            }
        }
    }
    free(dist);
    free(queue);
    return (seq_t)result;
}
