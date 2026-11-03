/*
 * A320195  Number of integer solutions to a^2 + 2*b^2 + 4*c^2 + 6*d^2 = n.
 * https://oeis.org/A320195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of integer solutions to a^2 + 2*b^2 + 4*c^2 + 6*d^2 = n
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9934 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for calloc, free */
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t size = n + 1;
    seq_t *cur  = mem_new(seq_t, size);
    seq_t *next = mem_new(seq_t, size);
    if (!cur || !next) {
        /* memory allocation failed – return 0 as sentinel */
        mem_free(cur); mem_free(next);
        return (seq_t)0;
    }

    cur[0] = (seq_t)1;  /* start with the empty sum */

    const idx_t coeffs[4] = {1, 2, 4, 6};

    for (int ci = 0; ci < 4; ++ci) {
        mem_set(next, 0, size * sizeof(seq_t));

        idx_t coeff = coeffs[ci];

        for (idx_t t = 0;; ++t) {
            __int128 v128 = (__int128)coeff * t * t;
            if (v128 > n) break;
            idx_t val = (idx_t)v128;

            seq_t cnt = (t == 0) ? (seq_t)1 : (seq_t)2;

            for (idx_t s = 0; s + val <= n; ++s) {
                seq_t cs = cur[s];
                if (cs) {
                    next[s + val] += cs * cnt;
                }
            }
        }

        /* swap cur and next for the next coefficient */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    seq_t result = cur[n];
    mem_free(cur);
    mem_free(next);
    return result;
}
