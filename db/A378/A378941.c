/*
 * A378941  Number of Motzkin paths of length n up to reversal.
 * https://oeis.org/A378941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A001006(n) + A005773(floor(1+n/2)))/2, where A001006(n)
 *            = Motzkin number and A005773(m) = number of Motzkin path
 *            prefixes of length m-1 (any end height). Computed with one
 *            height DP using additions only
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 45, u128 n <= 86,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 848 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    idx_t H = n / 2 + 1;
    idx_t half = n / 2;
    seq_t *cur = mem_new(seq_t, (size_t)(H + 2));
    seq_t *nxt = mem_new(seq_t, (size_t)(H + 2));
    seq_t B = 0, M;
    idx_t s, h;
    cur[0] = 1;
    for (s = 0; s <= n; s++) {
        if (s == half) {
            B = 0;
            for (h = 0; h <= H; h++) B += cur[h];
        }
        if (s == n) break;
        for (h = 0; h <= H; h++) {
            seq_t v = cur[h] + cur[h + 1];
            if (h > 0) v += cur[h - 1];
            nxt[h] = v;
        }
        nxt[H + 1] = 0;
        { seq_t *t = cur; cur = nxt; nxt = t; }
    }
    M = cur[0];
    mem_free(cur);
    mem_free(nxt);
    /* M and B have the same parity (M + B is even) */
    return M / 2 + B / 2 + (M & B & 1);
}
