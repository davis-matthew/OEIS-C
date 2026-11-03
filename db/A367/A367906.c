/*
 * A367906  Numbers k such that it is possible to choose a different binary
 *          index of each binary index of k.
 * https://oeis.org/A367906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 121, u16 n <= 3541, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9128 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: brute‑force search for BII numbers using bipartite matching */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* ##### Helper: bipartite matching (Kuhn algorithm) ##### */
static bool kuhn_dfs(int u, int *matchR, int rightCnt,
                     int Adj[64][64], int *AdjCnt, bool *seen)
{
    for (int i = 0; i < AdjCnt[u]; ++i) {
        int v = Adj[u][i];
        if (seen[v]) continue;
        seen[v] = true;
        if (matchR[v] == -1 || kuhn_dfs(matchR[v], matchR, rightCnt,
                                        Adj, AdjCnt, seen)) {
            matchR[v] = u;
            return true;
        }
    }
    return false;
}

/* ##### Helper: test whether a number k is a BII‑number ##### */
static bool is_bii(idx_t k)
{
    /* Identify the positions of 1‑bits in k (positions start at 1). */
    int left[64];
    int m = 0;                       /* number of left nodes */
    uint64_t temp = k;
    int pos = 1;
    while (temp) {
        if (temp & 1ULL) left[m++] = pos;
        temp >>= 1;
        pos++;
    }
    if (m == 0) return false;        /* k == 0 cannot occur for n ≥ 1 */

    /* Right nodes: distinct binary indices of the left nodes. */
    int rightVals[64];
    int rightCnt = 0;

    /* Adjacency: left u → right indices. */
    int Adj[64][64];
    int AdjCnt[64];
    for (int i = 0; i < m; ++i) AdjCnt[i] = 0;

    for (int u = 0; u < m; ++u) {
        int v = left[u];
        uint64_t t = (uint64_t)v;
        int p = 1;
        while (t) {
            if (t & 1ULL) {
                int ridx = -1;
                for (int a = 0; a < rightCnt; ++a)
                    if (rightVals[a] == p) { ridx = a; break; }
                if (ridx == -1) {
                    ridx = rightCnt;
                    rightVals[rightCnt++] = p;
                }
                Adj[u][AdjCnt[u]++] = ridx;
            }
            t >>= 1;
            p++;
        }
    }

    if (m > rightCnt) return false;   /* left side cannot match right side */

    /* Bipartite matching. */
    int matchR[64];
    for (int v = 0; v < rightCnt; ++v) matchR[v] = -1;
    bool seen[64];

    for (int u = 0; u < m; ++u) {
        for (int v = 0; v < rightCnt; ++v) seen[v] = false;
        if (!kuhn_dfs(u, matchR, rightCnt, Adj, AdjCnt, seen))
            return false;
    }
    return true;
}

/* ##### Main sequence function ##### */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* allocated capacity */
    static size_t count = 0;     /* number of stored terms   */
    static idx_t last_k_checked = 0; /* largest k already tested */

    /* Produce terms until we have at least n of them. */
    size_t target = (size_t)n;
    while (count < target) {
        idx_t k = ++last_k_checked;

        if (is_bii(k)) {
            if (count == cap) {
                size_t new_cap = (cap == 0) ? 8 : cap << 1;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return (seq_t)0;
                cache = tmp;
                cap = new_cap;
            }
            cache[count++] = (seq_t)k;
        }
    }
    return cache[n - 1];
}
