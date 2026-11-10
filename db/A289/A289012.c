/*
 * A289012  Positions of 0 in A289011; complement of A289013.
 * https://oeis.org/A289012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A289011, the fixed point of
 *            00->0010, 01->001, 10->100 starting with 00; computed by
 *            iterating the morphism until n zeros appear
 * Range:     u8 n <= 180, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9902 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t need = (size_t)n;
    size_t target = need * 2 + 16;
    for (;;) {
        size_t cap = 64;
        while (cap < target * 2 + 8) cap *= 2;
        uint8_t *cur  = (uint8_t *)malloc(cap);
        uint8_t *next = (uint8_t *)malloc(cap * 2);
        cur[0] = 0; cur[1] = 0;
        size_t len = 2;
        while (len < target) {
            size_t pairs = len / 2, pos = 0;
            for (size_t i = 0; i < pairs; ++i) {
                uint8_t x = cur[2 * i], y = cur[2 * i + 1];
                if (x == 0 && y == 0) {
                    next[pos++] = 0; next[pos++] = 0; next[pos++] = 1; next[pos++] = 0;
                } else if (x == 0 && y == 1) {
                    next[pos++] = 0; next[pos++] = 0; next[pos++] = 1;
                } else if (x == 1 && y == 0) {
                    next[pos++] = 1; next[pos++] = 0; next[pos++] = 0;
                } else {
                    next[pos++] = 1; next[pos++] = 1;
                }
            }
            if (len % 2) next[pos++] = cur[len - 1];
            if (pos > cap) {
                cap = pos;
                cur = (uint8_t *)realloc(cur, cap);
            }
            for (size_t i = 0; i < pos; ++i) cur[i] = next[i];
            len = pos;
            if (len > cap) break;
            if (len * 2 + 8 > cap * 2) {
                next = (uint8_t *)realloc(next, len * 4 + 16);
            }
        }
        size_t count = 0, res = 0;
        for (size_t i = 0; i < len; ++i) {
            if (cur[i] == 0) {
                if (++count == need) { res = i + 1; break; }
            }
        }
        free(cur);
        free(next);
        if (res) return (seq_t)res;
        target *= 2;
    }
}
