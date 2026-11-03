/*
 * A326947  BII-numbers of T_0 set-systems.
 * https://oeis.org/A326947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th BII‑number of a T₀ set‑system
 * Range:     u8 n <= 224, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 8752 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 1, a(3) = 2, a(4) = 3, … */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static bool BII_is_T0(idx_t cand)
{
    if (cand == 0) return true;          /* the empty set‑system */

    /* vertex_masks[v] keeps a bitmask of block indices that contain vertex v (1‑based) */
    uint64_t vertex_masks[65] = {0};
    idx_t temp = cand;
    int pos = 0;            /* position of the bit in cand (0‑based) */
    int block_idx = 0;      /* index of current block (0‑based) */

    while (temp != 0) {
        if (temp & 1ULL) {
            /* i = position of the 1‑bit, converted to 1‑based integer */
            idx_t i = pos + 1;
            idx_t vtemp = i;
            int q = 0;      /* bit position inside i (0‑based) */
            while (vtemp != 0) {
                if (vtemp & 1ULL) {
                    int v = q + 1;   /* vertex index (1‑based) */
                    if (v < 65) {
                        vertex_masks[v] |= (1ULL << block_idx);
                    }
                }
                vtemp >>= 1;
                q++;
            }
            block_idx++;
        }
        temp >>= 1;
        pos++;
    }

    /* check that all non‑zero vertex masks are distinct */
    for (int v1 = 1; v1 < 65; ++v1) {
        if (vertex_masks[v1] == 0) continue;
        for (int v2 = v1 + 1; v2 < 65; ++v2) {
            if (vertex_masks[v2] == vertex_masks[v1]) {
                return false;   /* duplicate dual block → not T₀ */
            }
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not used, but safe */

    /* static cache for already generated terms */
    static idx_t seq_len = 0;             /* number of cached terms */
    static idx_t seq_cap = 0;             /* capacity of the array */
    static seq_t *seq = NULL;             /* cached sequence */
    static idx_t next_candidate = 0;      /* next BII-number to test */

    while (seq_len < n) {
        while (true) {
            if (BII_is_T0(next_candidate)) {
                if (seq_len == seq_cap) {
                    seq_cap = seq_cap == 0 ? 1 : seq_cap << 1;
                    seq = mem_resize(seq_t, seq, seq_cap);
                }
                seq[seq_len] = (seq_t)next_candidate;
                seq_len++;
                next_candidate++;
                break;
            }
            next_candidate++;
        }
    }
    return seq[n - 1];
}
