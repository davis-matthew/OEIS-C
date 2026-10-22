/*
 * A327098  BII-numbers of set-systems with cut-connectivity 1.
 * https://oeis.org/A327098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A327098: BII-numbers of set-systems with cut-connectivity 1
   Computation: for each integer n build the set-system defined by
   binary indices of n and check whether its minimum vertex cut has
   cardinality 1.  Cut-connectivity 1 holds iff the hypergraph is
   connected and either the single vertex exception applies or the
   adjacency graph has an articulation point.  The sequence values
   themselves are the BII-numbers. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Maximum possible vertex index for n up to 10 000 */
#define MAX_V 128

/* Number of pre‑allocated terms in the cache */
#define CACHE_CAP 50000

static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;
static idx_t last_checked = 0;

/* Recursive DFS for articulation point detection                     */
static void dfsArt(int u, int parent, int root,
                   const int *adjSize, const int adj[][MAX_V+1],
                   int *disc, int *low,
                   int *timeCnt, bool *hasArt, int *rootChildCnt)
{
    disc[u] = (*timeCnt)++;
    low[u] = disc[u];

    for (int i = 0; i < adjSize[u]; ++i) {
        int v = adj[u][i];
        if (disc[v] == -1) {
            if (u == root) (*rootChildCnt)++;
            dfsArt(v, u, root, adjSize, adj,
                   disc, low, timeCnt, hasArt, rootChildCnt);

            if (low[v] >= disc[u]) {
                if (u != root) *hasArt = true;
            }
            if (low[v] < low[u]) low[u] = low[v];
        } else if (v != parent) {
            if (disc[v] < low[u]) low[u] = disc[v];
        }
    }
}

/* Test whether n is a BII-number of a set-system with cut-connectivity 1 */
static bool qualifies(uint64_t n)
{
    /* 1. Binary indices of n (positions of 1‑bits) */
    uint64_t idx[64];
    int idxCnt = 0;
    uint64_t t = n;
    unsigned pos = 1;
    while (t) {
        if (t & 1ULL) idx[idxCnt++] = pos;
        t >>= 1;
        ++pos;
    }

    /* 2. Build the sets: binary indices of each index */
    int sets[64][MAX_V+1];
    int setSize[64];
    int setCnt = 0;
    int maxV    = 0;

    for (int i = 0; i < idxCnt; ++i) {
        uint64_t pi  = idx[i];
        uint64_t u   = pi;
        unsigned b   = 1;
        int sz       = 0;
        while (u) {
            if (u & 1ULL) {
                sets[setCnt][sz++] = (int)b;
                if ((int)b > maxV) maxV = (int)b;
            }
            u >>= 1;
            ++b;
        }
        setSize[setCnt++] = sz;
    }
    if (maxV == 0) return false;  /* never happens for n>0 */

    /* 3. Build adjacency graph and record present vertices */
    int adjSize[MAX_V+1] = {0};
    int adj[MAX_V+1][MAX_V+1] = {0};
    int present[MAX_V+1]      = {0};

    for (int i = 0; i < setCnt; ++i) {
        int sz = setSize[i];
        for (int j = 0; j < sz; ++j) {
            int v = sets[i][j];
            if (v <= MAX_V) present[v] = 1;
        }
        for (int a = 0; a < sz; ++a)
            for (int b = a+1; b < sz; ++b) {
                int u = sets[i][a];
                int v = sets[i][b];
                if (u == v) continue;
                if (adjSize[u] < MAX_V) adj[u][adjSize[u]++] = v;
                if (adjSize[v] < MAX_V) adj[v][adjSize[v]++] = u;
            }
    }

    /* 4. Count vertices and find a starting point */
    int vertCnt = 0;
    int firstV  = 0;
    for (int v = 1; v <= maxV; ++v) {
        if (present[v]) {
            ++vertCnt;
            if (!firstV) firstV = v;
        }
    }
    if (vertCnt == 0) return false;

    if (vertCnt == 1) return true;          /* single‑vertex exception */

    /* 5. Check initial connectivity */
    int visited[MAX_V+1] = {0};
    int stack[MAX_V+1];
    int top = 0;
    visited[firstV] = 1;
    stack[top++] = firstV;
    int visCnt = 0;
    while (top) {
        int u = stack[--top];
        ++visCnt;
        for (int i = 0; i < adjSize[u]; ++i) {
            int w = adj[u][i];
            if (present[w] && !visited[w]) {
                visited[w] = 1;
                stack[top++] = w;
            }
        }
    }
    if (visCnt < vertCnt) return false;     /* already disconnected */

    /* 6. Vertex connectivity: articulation point detection */
    int disc[MAX_V+1];
    int low[MAX_V+1];
    for (int v = 1; v <= maxV; ++v) disc[v] = -1;
    int timeCnt   = 0;
    bool hasArt   = false;
    int rootChild = 0;

    dfsArt(firstV, -1, firstV, adjSize, adj,
           disc, low, &timeCnt, &hasArt, &rootChild);
    if (rootChild > 1) hasArt = true;

    return hasArt;
}

/* Required public function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range, return 0 */

    while (cache_len < (size_t)n) {
        ++last_checked;
        if (qualifies((uint64_t)last_checked)) {
            if (cache_len < CACHE_CAP)
                cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n-1];
}
