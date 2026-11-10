/*
 * A284946  Positions of 1 in A284944; complement of A284945.
 * https://oeis.org/A284946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A284944, the fixed point of
 *            0->01, 1->1110; found by descending the morphism levels using
 *            per-level counts of ones and lengths
 * Range:     u8 n <= 180, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 6756 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    enum { MAXD = 200 };
    seq_t *len0 = mem_new(seq_t, MAXD);
    seq_t *len1 = mem_new(seq_t, MAXD);
    seq_t *on0 = mem_new(seq_t, MAXD);
    seq_t *on1 = mem_new(seq_t, MAXD);
    len0[0] = 1; len1[0] = 1;
    on0[0] = 0; on1[0] = 1;
    size_t d = 0;
    while (on0[d] < (seq_t)n && d + 1 < MAXD) {
        ++d;
        len0[d] = len0[d - 1] + len1[d - 1];
        len1[d] = 3 * len1[d - 1] + len0[d - 1];
        on0[d] = on0[d - 1] + on1[d - 1];
        on1[d] = 3 * on1[d - 1] + on0[d - 1];
    }
    seq_t k = (seq_t)n;
    seq_t pos = 0;
    int letter = 0;
    while (d > 0) {
        if (letter == 0) {
            if (k <= on0[d - 1]) {
                letter = 0;
            } else {
                k -= on0[d - 1];
                pos += len0[d - 1];
                letter = 1;
            }
        } else {
            int i;
            for (i = 0; i < 3; i++) {
                if (k <= on1[d - 1]) break;
                k -= on1[d - 1];
                pos += len1[d - 1];
            }
            letter = (i == 3) ? 0 : 1;
        }
        --d;
    }
    seq_t res = pos + 1;
    mem_free(len0); mem_free(len1); mem_free(on0); mem_free(on1);
    return res;
}
