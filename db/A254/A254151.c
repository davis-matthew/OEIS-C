/*
 * A254151  Number of independent sets in the generalized Aztec diamond
 *          E(L_7,L_{2n-1}).
 * https://oeis.org/A254151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A254151: Number of independent sets in the generalized
   Aztec diamond E(L_7,L_{2n-1}).  The computation uses a linear‑time
   dynamic program on the alternating 4‑vertex and 3‑vertex layers of the
   graph.  Initial values: a(0)=1, a(1)=16, a(2)=314, … (see OEIS). */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Compatibility table for an odd layer (4 vertices) and an even layer
   (3 vertices).  compat[m4][m3] is true iff the selections described
   by the bit masks m4 (4 bits) and m3 (3 bits) contain no adjacent
   vertices.  The table is filled once at first use. */
static bool compat[16][8];
static bool compat_inited = false;

static void init_compat(void) {
    /* Neighbor masks for the four vertices of an odd layer.  Vertex i
       forbids the vertices listed in conflictMasks4[i] of the adjacent
       even layer.  The masks are computed from the explicit graph
       adjacency. */
    static const uint8_t conflictMasks4[4] = {1, 3, 6, 4};
    for (uint8_t m4 = 0; m4 < 16; ++m4) {
        for (uint8_t m3 = 0; m3 < 8; ++m3) {
            bool ok = true;
            for (int i = 0; i < 4 && ok; ++i) {
                if (m4 & (1u << i)) {
                    uint8_t bad = conflictMasks4[i];
                    if (m3 & bad) ok = false;
                }
            }
            compat[m4][m3] = ok;
        }
    }
    compat_inited = true;
}

seq_t a(idx_t n) {
    /* Zero index: empty graph */
    if (n == 0) return (seq_t)1;

    /* Number of layers (b‑coordinates) = 2n – 1 (first is odd).  n ≥ 1 here. */
    idx_t layers = (idx_t)2 * n - 1;

    if (!compat_inited) init_compat();

    /* DP arrays for the current layer states.  dp4 holds counts for
       odd layers (4 bits), dp3 for even layers (3 bits). */
    seq_t dp4[16] = {0};
    seq_t dp3[8]  = {0};
    seq_t new_dp4[16] = {0};
    seq_t new_dp3[8]  = {0};

    /* First layer is odd: any subset of the 4 vertices is allowed. */
    for (int m = 0; m < 16; ++m) dp4[m] = (seq_t)1;

    /* Process remaining layers.  layer indexes are 1‑based; layer 1 is
       already handled. */
    for (idx_t layer = 2; layer <= layers; ++layer) {
        if ((layer & 1) == 0) {        /* even layer – 3 vertices */
            /* Compute new counts for each m3 from all previous m4. */
            for (int m3 = 0; m3 < 8; ++m3) {
                seq_t sum = 0;
                for (int m4 = 0; m4 < 16; ++m4) {
                    if (compat[m4][m3]) sum += dp4[m4];
                }
                new_dp3[m3] = sum;
            }
            /* Copy new counts into dp3 and reset dp4. */
            for (int i = 0; i < 8; ++i) dp3[i] = new_dp3[i];
            for (int i = 0; i < 16; ++i) dp4[i] = 0;
        } else {                      /* odd layer – 4 vertices */
            for (int m4 = 0; m4 < 16; ++m4) {
                seq_t sum = 0;
                for (int m3 = 0; m3 < 8; ++m3) {
                    if (compat[m4][m3]) sum += dp3[m3];
                }
                new_dp4[m4] = sum;
            }
            for (int i = 0; i < 16; ++i) dp4[i] = new_dp4[i];
            for (int i = 0; i < 8; ++i) dp3[i] = 0;
        }
    }

    /* Sum the counts of all states in the final odd layer. */
    seq_t total = 0;
    for (int m = 0; m < 16; ++m) total += dp4[m];
    return total;
}
