/*
 * A327097  BII-numbers of set-systems with non-spanning edge-connectivity 2.
 * https://oeis.org/A327097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer whose BII-number yields a set-system with
 *            non‑spanning edge‑connectivity 2
 * Range:     u8 n <= 43, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Union–find helpers -----------------------------------------------------*/
static inline int uf_find(int *parent, int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}
static inline void uf_union(int *parent, int a, int b) {
    int ra = uf_find(parent, a);
    int rb = uf_find(parent, b);
    if (ra != rb) parent[rb] = ra;
}

/* Test if a hypergraph given by edge masks is connected (non‑empty). */
static inline bool is_connected(const idx_t *edges, int k) {
    if (k == 0) return false;                   /* no edges */
    idx_t allVerts = 0;
    for (int i = 0; i < k; ++i) allVerts |= edges[i];
    if (allVerts == 0) return false;            /* no vertices */

    int parent[64];
    for (int v = 0; v < 64; ++v) parent[v] = v;

    /* Union vertices that belong to the same edge (clique). */
    for (int i = 0; i < k; ++i) {
        idx_t m = edges[i];
        if (m == 0) continue;
        int first = __builtin_ctzll(m) & 63;    /* least significant set bit */
        idx_t rest = m & (m - 1);
        while (rest) {
            int v = __builtin_ctzll(rest) & 63;
            uf_union(parent, first, v);
            rest &= rest - 1;
        }
    }

    /* All vertices must belong to the same component. */
    int rep = uf_find(parent, __builtin_ctzll(allVerts) & 63);
    idx_t m = allVerts;
    while (m) {
        int v = __builtin_ctzll(m) & 63;
        if (uf_find(parent, v) != rep) return false;
        m &= m - 1;
    }
    return true;
}

/* Return true if the set–system derived from n has non‑spanning edge‐connectivity 2. */
static bool qualifies(idx_t n) {
    /* 1. Compute positions of set bits (binary indices). */
    idx_t tmp = n;
    int pos = 1;
    int idxs[64], icnt = 0;
    while (tmp) {
        if (tmp & 1ULL) idxs[icnt++] = pos;
        tmp >>= 1; ++pos;
    }

    /* 2. Build edge masks: for each index, mask of set bits of that index. */
    idx_t edges[64]; int k = 0;
    for (int i = 0; i < icnt; ++i) {
        idx_t val = idxs[i];
        int p = 1;
        idx_t mask = 0;
        while (val) {
            if (val & 1ULL) mask |= 1ULL << (p - 1);
            val >>= 1; ++p;
        }
        if (mask) edges[k++] = mask;
    }
    if (k == 0) return false;          /* should not happen for n>0 */

    /* 3. Must be initially connected. */
    if (!is_connected(edges, k)) return false;
    if (k == 1) return false;          /* edge‑connectivity 1 */

    /* 4. No single edge removal may disconnect (connectivity >1). */
    idx_t rem[64];
    for (int i = 0; i < k; ++i) {
        int r = 0;
        for (int j = 0; j < k; ++j)
            if (j != i) rem[r++] = edges[j];
        if (!is_connected(rem, r)) return false;   /* disconnected by one edge */
    }

    /* 5. There must exist a pair of edges whose removal disconnects (connectivity 2). */
    for (int i = 0; i < k; ++i)
        for (int j = i + 1; j < k; ++j) {
            int r = 0;
            for (int t = 0; t < k; ++t)
                if (t != i && t != j) rem[r++] = edges[t];
            if (!is_connected(rem, r)) return true;   /* disconnected by two edges */
        }
    return false;   /* no pair disconnects → connectivity >2 */
}

/* Return the n-th member of the sequence. */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* offset is 1; undefined for 0 */

    idx_t count = 0;
    idx_t cand  = 1;
    while (count < n) {
        if (qualifies(cand)) ++count;
        ++cand;
    }
    return (seq_t)(cand - 1);
}
