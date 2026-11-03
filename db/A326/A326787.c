/*
 * A326787  Non-spanning edge-connectivity of the set-system with BII-number
 *          n.
 * https://oeis.org/A326787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimum number of edges that must be removed from * the
 *            set-system derived from n's binary indices to make it *
 *            disconnected or empty. Computed by brute‑forcing subsets of *
 *            the edges of the associated edge‑graph
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 86 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* utility: mask of binary indices of x (positions of 1 bits, 1‑based) */
static inline uint64_t binary_indices_mask(idx_t x)
{
    uint64_t mask = 0;
    uint8_t pos = 1;
    while (x) {
        if (x & 1) mask |= 1ULL << (pos - 1);
        x >>= 1;
        ++pos;
    }
    return mask;
}

/* utility: test if the remaining nodes after removing removedMask are
 * disconnected (or none left).  adj[i] contains bitmask of neighbors of i. */
static inline bool is_disconnected(const uint64_t adj[], int E,
                                   uint64_t removedMask)
{
    uint64_t remMask = (~removedMask) & ((E == 64) ? ~0ULL
                         : ((1ULL << E) - 1));
    if (remMask == 0) return true;              /* empty graph */
    int start = __builtin_ctzll(remMask);        /* first remaining node */

    uint64_t visited = 0;
    uint64_t stack[64];
    int sp = 0;
    stack[sp++] = start;
    visited |= 1ULL << start;

    while (sp) {
        int v = (int)stack[--sp];
        uint64_t neigh = adj[v] & remMask;
        while (neigh) {
            int u = __builtin_ctzll(neigh);
            neigh &= neigh - 1;
            if (!(visited & (1ULL << u))) {
                visited |= 1ULL << u;
                stack[sp++] = u;
            }
        }
    }
    return visited != remMask;
}

seq_t a(idx_t n)
{
    /* collect edges: each edge is the binary indices of a bit position of n */
    uint64_t edges[64];
    int E = 0;

    for (uint8_t pos = 1; pos <= 64 && n; ++pos) {
        if (n & 1ULL) {
            if (E < 64)   /* safety, though for n < 10k E will be < 15 */
                edges[E++] = binary_indices_mask(pos);
        }
        n >>= 1;
    }

    if (E == 0) return 0;          /* no edges → already empty */
    if (E == 1) return 1;          /* single edge → needs its removal */

    /* build adjacency of the edge‑graph */
    uint64_t adj[64] = {0};
    for (int i = 0; i < E; ++i)
        for (int j = i + 1; j < E; ++j)
            if (edges[i] & edges[j]) {
                adj[i] |= 1ULL << j;
                adj[j] |= 1ULL << i;
            }

    /* check if already disconnected */
    uint64_t fullMask = (E == 64) ? ~0ULL : ((1ULL << E) - 1);
    if (is_disconnected(adj, E, 0)) return 0;

    /* brute‑force minimum vertex cut */
    seq_t ans = (seq_t)E;           /* upper bound */
    for (uint64_t mask = 1; mask <= fullMask; ++mask) {
        int pc = __builtin_popcountll(mask);
        if (pc >= ans) continue;    /* no better than current best */
        if (is_disconnected(adj, E, mask)) {
            ans = pc;
            if (ans == 1) break;    /* cannot get smaller */
        }
    }
    return ans;
}
