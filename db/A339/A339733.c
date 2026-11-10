/*
 * A339733  Square array T(n, k) read by antidiagonals, n > 0 and k > 0; let G
 *          be the undirected graph with nodes {g_k, k > 0} such that for any
 *          k > 0, g_k is connected to g_{k+1} and g_{A064413(k)} is connected
 *          to g_{A064413(k+1)}; T(n, k) is the distance between g_n and g_k.
 * https://oeis.org/A339733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   search - build the EKG sequence A064413 (a(1)=1, a(2)=2,
 *            a(n)=smallest unused number sharing a factor with a(n-1)), then
 *            breadth-first search the graph with edges k~k+1 and
 *            A064413(k)~A064413(k+1) for the distance between g_n and g_k
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t m)
{
    uint64_t d = (uint64_t)__builtin_sqrt(2.0 * (double)m);
    if (d < 1) d = 1;
    while (d * (d + 1) / 2 < m) d++;
    while (d > 1 && d * (d - 1) / 2 >= m) d--;
    uint64_t i = m - d * (d - 1) / 2;
    uint32_t n = (uint32_t)i;
    uint32_t k = (uint32_t)(d + 1 - i);
    if (n == k) return (seq_t)0;
    uint32_t mx = n > k ? n : k;
    uint32_t L = 4 * mx + 1000;
    uint32_t V = 3 * L + 10;

    uint32_t *E = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    uint32_t *inv = (uint32_t *)calloc(V + 2, sizeof(uint32_t));
    uint32_t *ptr = (uint32_t *)calloc(V + 2, sizeof(uint32_t));
    unsigned char *used = (unsigned char *)calloc(V + 2, 1);
    int *dist = (int *)malloc((V + 2) * sizeof(int));
    uint32_t *queue = (uint32_t *)malloc((V + 2) * sizeof(uint32_t));

    E[1] = 1; E[2] = 2; used[1] = used[2] = 1;
    inv[1] = 1; inv[2] = 2;
    uint32_t last = L;
    for (uint32_t t = 3; t <= L; t++) {
        uint32_t prev = E[t - 1], x = prev, best = 0xFFFFFFFFu;
        for (uint32_t p = 2; p * p <= x || x > 1; p++) {
            if (p * p > x) p = x;
            if (x % p == 0) {
                while (x % p == 0) x /= p;
                uint32_t c = ptr[p] ? ptr[p] : p;
                while (c <= V && used[c]) c += p;
                ptr[p] = c;
                if (c < best) best = c;
            }
            if (x == 1) break;
        }
        if (best > V) { last = t - 1; break; }
        used[best] = 1;
        E[t] = best;
        inv[best] = t;
    }

    for (uint32_t v = 0; v <= V + 1; v++) dist[v] = -1;
    size_t qh = 0, qt = 0;
    dist[n] = 0;
    queue[qt++] = n;
    int result = (int)(n > k ? n - k : k - n);
    while (qh < qt) {
        uint32_t v = queue[qh++];
        if (v == k) { result = dist[v]; break; }
        uint32_t nb[4];
        int c = 0;
        if (v > 1) nb[c++] = v - 1;
        if (v + 1 <= V) nb[c++] = v + 1;
        uint32_t p = inv[v];
        if (p) {
            if (p > 1) nb[c++] = E[p - 1];
            if (p + 1 <= last) nb[c++] = E[p + 1];
        }
        for (int j = 0; j < c; j++) {
            uint32_t w = nb[j];
            if (dist[w] < 0) {
                dist[w] = dist[v] + 1;
                queue[qt++] = w;
            }
        }
    }
    free(E); free(inv); free(ptr); free(used); free(dist); free(queue);
    return (seq_t)result;
}
