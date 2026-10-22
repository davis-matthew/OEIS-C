/*
 * A384104  Number of edge-connected components of n faces of the truncated
 *          tetrahedron up to the 24 rotations and reflections of the
 *          truncated tetrahedron.
 * https://oeis.org/A384104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   generate_group
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula implemented: number of edge-connected components (free polyforms)
   of n faces of the truncated tetrahedron, up to the 24 rotations
   and reflections.  The truncated tetrahedron has 8 faces: 4
   triangular (indices 0‑3) and 4 hexagonal (indices 4‑7).
   The face‑adjacency graph is:
   • Each triangle is adjacent to the three hexagons with different
     indices (i ≠ j).
   • Each hexagon is adjacent to the three triangles that are not its
     own index and to the other three hexagons.
   The symmetry group is the full tetrahedral group S₄; it permutes the
   four triangle indices and the same permutation is applied to the four
   hexagon indices.  Subsets of faces that induce a connected
   subgraph are counted up to symmetry. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/generate_group.h"

static uint8_t adjacency[8];

static void init_adjacency(void) {
    for (int i = 0; i < 8; ++i) adjacency[i] = 0;
    /* triangle (0‑3) ↔ hexagon (4‑7) where indices differ */
    for (int t = 0; t < 4; ++t)
        for (int h = 0; h < 4; ++h)
            if (t != h) {
                adjacency[t]       |= 1u << (4 + h);
                adjacency[4 + h]   |= 1u << t;
            }
    /* hexagon ↔ hexagon (K4) */
    for (int h1 = 0; h1 < 4; ++h1)
        for (int h2 = h1 + 1; h2 < 4; ++h2) {
            adjacency[4 + h1] |= 1u << (4 + h2);
            adjacency[4 + h2] |= 1u << (4 + h1);
        }
}

static bool is_connected(uint8_t mask) {
    if (mask == 0) return true;
    int start = -1;
    for (int v = 0; v < 8; ++v) if (mask & (1u << v)) { start = v; break; }
    uint8_t frontier = 1u << start;
    uint8_t visited  = frontier;
    while (frontier) {
        uint8_t next = 0;
        for (int v = 0; v < 8; ++v) {
            if (frontier & (1u << v))
                next |= adjacency[v];
        }
        next &= mask;
        next &= ~visited;
        if (!next) break;
        frontier = next;
        visited |= next;
    }
    return visited == mask;
}

seq_t a(idx_t n) {
    if (n > 8) return 0;                 /* only 8 faces are available */

    static bool initialized = false;
    static uint8_t group[24][8];
    if (!initialized) {
        generate_group(group);
        init_adjacency();
        initialized = true;
    }

    seq_t answer = 0;
    bool seen[256] = {false};

    for (uint16_t mask = 0; mask < 256; ++mask) {
        /* popcount check */
        uint8_t pc = 0, tmp = mask;
        while (tmp) { tmp &= tmp - 1; ++pc; }
        if (pc != (uint8_t)n) continue;

        /* connectivity test */
        if (!is_connected((uint8_t)mask)) continue;

        /* canonical form under symmetry */
        uint8_t best = 0xFF;
        for (int g = 0; g < 24; ++g) {
            uint8_t img = 0;
            for (int v = 0; v < 8; ++v)
                if (mask & (1u << v))
                    img |= 1u << group[g][v];
            if (img < best) best = img;
        }
        if (!seen[best]) {
            seen[best] = true;
            ++answer;
        }
    }
    return answer;
}
