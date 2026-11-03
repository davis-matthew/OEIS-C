/*
 * A327100  BII-numbers of antichains of sets with cut-connectivity 1.
 * https://oeis.org/A327100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A327100 – BII-numbers of antichains of sets with cut‑connectivity 1  
   Computed by testing successive integers for the property described in
   the OEIS entry.  For each integer m we construct its binary‑index set system,
   verify that it is an antichain, that it is connected, and that it has an
   articulation point (vertex cut of size 1).  When such an integer is
   found it is appended to the static sequence cache.  The function a(n)
   returns the n‑th element (1‑based). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* --------------------------------------------------------------------- */
static void dfs_art_helper(int u, const uint64_t *adj, uint64_t vertex_mask,
                           bool *vis, int *disc, int *low, int *parent,
                           int *time, bool *found)
{
    vis[u] = true;
    disc[u] = low[u] = ++(*time);
    int childCount = 0;

    uint64_t nb = adj[u] & vertex_mask;
    while (nb) {
        int v = __builtin_ctzll(nb);
        if (!vis[v]) {
            parent[v] = u;
            childCount++;
            dfs_art_helper(v, adj, vertex_mask, vis, disc, low, parent,
                           time, found);
            if (low[v] < low[u])
                low[u] = low[v];
            if (parent[u] == -1 && childCount > 1)
                *found = true;
            if (parent[u] != -1 && low[v] >= disc[u])
                *found = true;
        } else if (v != parent[u]) {
            if (disc[v] < low[u])
                low[u] = disc[v];
        }
        nb &= nb - 1;
    }
}

/* --------------------------------------------------------------------- */
static bool is_bii_antichain_cutconn1(uint64_t m)
{
    if (m == 0)
        return false;

    /* binary indices of m (positions of set bits, 1‑based) */
    int idx[64];
    int idx_cnt = 0;
    uint64_t tmp = m;
    while (tmp) {
        int pos = __builtin_ctzll(tmp);          /* 0‑based */
        idx[idx_cnt++] = pos + 1;                /* 1‑based position */
        tmp &= tmp - 1;
    }

    /* build the set‑system: each set is the binary indices of its index */
    uint64_t sets[64];
    int set_cnt = 0;
    uint64_t vertex_mask = 0;
    for (int i = 0; i < idx_cnt; ++i) {
        uint64_t s = 0;
        uint64_t sub = (uint64_t)idx[i];
        while (sub) {
            int b = __builtin_ctzll(sub);
            s |= 1ULL << b;
            vertex_mask |= 1ULL << b;
            sub &= sub - 1;
        }
        sets[set_cnt++] = s;
    }

    /* antichain test: no set is subset of another */
    for (int i = 0; i < set_cnt; ++i) {
        for (int j = i + 1; j < set_cnt; ++j) {
            uint64_t a = sets[i], b = sets[j];
            if ((a & b) == a || (a & b) == b)
                return false;
        }
    }

    /* single‑vertex case is automatically valid */
    if ((vertex_mask & (vertex_mask - 1)) == 0)
        return true;

    /* adjacency graph from sets */
    uint64_t adj[64] = {0};
    for (int i = 0; i < set_cnt; ++i) {
        uint64_t s = sets[i];
        uint64_t s0 = s;
        while (s0) {
            int u = __builtin_ctzll(s0);
            uint64_t rem = s0 & ~(1ULL << u);
            uint64_t t = rem;
            while (t) {
                int v = __builtin_ctzll(t);
                adj[u] |= 1ULL << v;
                adj[v] |= 1ULL << u;
                t &= t - 1;
            }
            s0 &= s0 - 1;
        }
    }

    /* connectivity check (BFS) */
    int visited[64] = {0};
    int queue[64];
    int qh = 0, qt = 0;
    int start = __builtin_ctzll(vertex_mask);
    visited[start] = 1;
    queue[qt++] = start;
    int visited_cnt = 1;

    while (qh < qt) {
        int u = queue[qh++];
        uint64_t nb = adj[u] & vertex_mask;
        while (nb) {
            int v = __builtin_ctzll(nb);
            if (!visited[v]) {
                visited[v] = 1;
                queue[qt++] = v;
                visited_cnt++;
            }
            nb &= nb - 1;
        }
    }

    if (visited_cnt != 0) {
        int vertex_cnt = 0;
        uint64_t vm = vertex_mask;
        while (vm) { vertex_cnt++; vm &= vm - 1; }
        if (visited_cnt < vertex_cnt)
            return false;   /* disconnected */
    }

    /* articulation point detection */
    bool vis[64]   = {0};
    int disc[64]   = {0};
    int low[64]    = {0};
    int parent[64] = {0};
    for (int i = 0; i < 64; ++i) parent[i] = -1;
    int time = 0;
    bool found = false;

    dfs_art_helper(start, adj, vertex_mask, vis, disc, low, parent,
                   &time, &found);
    return found;
}

/* --------------------------------------------------------------------- */
/* Helper: depth‑first search for articulation points                    */

/* --------------------------------------------------------------------- */
/* Test whether an integer m satisfies the required BII‑antichain property */

/* --------------------------------------------------------------------- */
/* Main sequence generator                                             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache of generated terms */
    static seq_t *seq = NULL;
    static idx_t seq_len = 0;
    static idx_t seq_cap = 0;
    static uint64_t next = 1;      /* next integer candidate */

    while (seq_len < n) {
        /* grow cache if necessary */
        if (seq_len == seq_cap) {
            idx_t new_cap = seq_cap ? seq_cap * 2 : 4;
            seq = seq_realloc(seq, seq_cap, new_cap);
            seq_cap = new_cap;
        }

        /* find next qualifying integer */
        while (!is_bii_antichain_cutconn1(next))
            ++next;

        /* append to cache */
        seq[seq_len++] = (seq_t)next;
        ++next;
    }
    return seq[n - 1];
}
