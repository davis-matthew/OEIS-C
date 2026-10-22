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
 * Helpers:   binary_indices_mask, is_disconnected
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binary_indices_mask.h"
#include "../utils/is_disconnected.h"

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
