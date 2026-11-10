/*
 * A339697  Square array T(n, k) read by antidiagonals, n >= 0 and k >= 0; let
 *          G be the undirected graph with nodes {g_k, k >= 0} such that for
 *          any k >= 0, g_k is connected to g_{k+1} and g_{A006068(k)} is
 *          connected to g_{A006068(k+1)}; T(n, k) is the distance between g_n
 *          and g_k.
 * https://oeis.org/A339697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = BFS distance between g_n and g_k in the graph with
 *            edges k~k+1 and A006068(k)~A006068(k+1); a(idx) reads T by
 *            antidiagonals
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static unsigned long long A006068_a(unsigned long long n)
{
    unsigned long long r = n, s = n;
    while (s > 0) {
        s >>= 1;
        r ^= s;
    }
    return r;
}

static void add_nb(long long *q, long long *tail, int *dist, long long L, long long u, long long w, int du)
{
    if (w < 0 || w >= L) return;
    if (dist[w] >= 0) return;
    dist[w] = du + 1;
    q[(*tail)++] = w;
    (void)u;
}

seq_t a(idx_t n)
{
    unsigned long long idx = (unsigned long long)n;
    unsigned long long d = 0;
    while ((d + 1) * (d + 2) / 2 <= idx) d++;
    unsigned long long i = idx - d * (d + 1) / 2;
    unsigned long long x = i, y = d - i;
    unsigned long long mx = x > y ? x : y;
    if (x == y) return (seq_t)0;

    long long L = 2;
    while ((unsigned long long)L <= mx) L <<= 1;
    L <<= 1;

    int *dist = (int *)malloc(sizeof(int) * (size_t)L);
    long long *q = (long long *)malloc(sizeof(long long) * (size_t)L);
    long long i2;
    for (i2 = 0; i2 < L; i2++) dist[i2] = -1;
    long long head = 0, tail = 0;
    dist[x] = 0;
    q[tail++] = (long long)x;
    while (head < tail && dist[y] < 0) {
        long long v = q[head++];
        int dv = dist[v];
        add_nb(q, &tail, dist, L, v, v + 1, dv);
        add_nb(q, &tail, dist, L, v, v - 1, dv);
        unsigned long long p = (unsigned long long)v ^ ((unsigned long long)v >> 1);
        add_nb(q, &tail, dist, L, v, (long long)A006068_a(p + 1), dv);
        if (p > 0) add_nb(q, &tail, dist, L, v, (long long)A006068_a(p - 1), dv);
    }
    int res = dist[y];
    free(dist);
    free(q);
    return (seq_t)res;
}
