/*
 * A286060  Positions of 0 in A286059; complement of A286061.
 * https://oeis.org/A286060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A286059, the fixed point of
 *            00->001, 1->011 starting with 00 (computed by iterating the
 *            rewriting and scanning)
 * Range:     u8 n <= 115, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9723 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t target = (size_t)n;
    size_t cur_size = 2;
    uint8_t *cur = (uint8_t *)malloc(cur_size);
    if (!cur) return (seq_t)0;
    cur[0] = 0; cur[1] = 0;

    for (;;) {
        size_t zeros = 0;
        for (size_t i = 0; i < cur_size; i++) if (cur[i] == 0) zeros++;
        if (zeros >= target) break;

        uint8_t *next = (uint8_t *)malloc(cur_size * 3 + 5);
        if (!next) { free(cur); return (seq_t)0; }
        size_t ns = 0;
        for (size_t i = 0; i < cur_size; ) {
            if (cur[i] == 0 && i + 1 < cur_size && cur[i + 1] == 0) {
                next[ns++] = 0; next[ns++] = 0; next[ns++] = 1;
                i += 2;
            } else if (cur[i] == 1) {
                next[ns++] = 0; next[ns++] = 1; next[ns++] = 1;
                i += 1;
            } else {
                next[ns++] = 0;
                i += 1;
            }
        }
        free(cur);
        cur = next;
        cur_size = ns;
    }

    size_t cnt = 0, pos = 0;
    for (size_t i = 0; i < cur_size; i++) {
        if (cur[i] == 0) {
            cnt++;
            if (cnt == target) { pos = i + 1; break; }
        }
    }
    free(cur);
    return (seq_t)pos;
}
