/*
 * A284509  Positions of 0 in A284508; complement of A284510.
 * https://oeis.org/A284509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A284508, the even-iteration
 *            limit word of 0->1, 1->0001; computed by generating the word
 * Range:     u8 n <= 151, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9823 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t N = (size_t)n;
    size_t L = 2 * N + 16;
    for (;;) {
        uint8_t *cur = (uint8_t *)malloc(L);
        uint8_t *next = (uint8_t *)malloc(L);
        if (!cur || !next) {
            free(cur);
            free(next);
            return 0;
        }
        cur[0] = 0;
        size_t cur_len = 1;
        unsigned iter = 0;
        while (cur_len < L || (iter & 1u)) {
            size_t nl = 0;
            for (size_t i = 0; i < cur_len && nl < L; ++i) {
                if (cur[i] == 0) {
                    next[nl++] = 1;
                } else {
                    next[nl++] = 0;
                    if (nl < L) next[nl++] = 0;
                    if (nl < L) next[nl++] = 0;
                    if (nl < L) next[nl++] = 1;
                }
            }
            ++iter;
            cur_len = nl;
            uint8_t *t = cur; cur = next; next = t;
        }
        size_t cnt = 0;
        size_t pos = 0;
        for (size_t i = 0; i < cur_len; ++i) {
            if (cur[i] == 0) {
                if (++cnt == N) { pos = i + 1; break; }
            }
        }
        free(cur);
        free(next);
        if (pos) return (seq_t)pos;
        L *= 2;
    }
}
