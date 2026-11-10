/*
 * A289013  Positions of 1 in A289011; complement of A289012.
 * https://oeis.org/A289013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A289011, the fixed point of
 *            00->0010, 01->001, 10->100 starting from 00. Computed by
 *            iterating the morphism until the word has at least n ones
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t cap = 64;
    uint8_t *cur = (uint8_t *)malloc(cap);
    uint8_t *nxt = (uint8_t *)malloc(cap);
    cur[0] = 0; cur[1] = 0;
    size_t len = 2;
    uint64_t ones = 0;

    while (ones < (uint64_t)n) {
        size_t pairs = len / 2;
        size_t new_len = 0;
        for (size_t i = 0; i < pairs; ++i) {
            uint8_t x = cur[2 * i], y = cur[2 * i + 1];
            if (x == 0 && y == 0) new_len += 4;
            else if (x == 1 && y == 1) new_len += 2;
            else new_len += 3;
        }
        if (len % 2) new_len += 1;
        if (new_len > cap) {
            cap = new_len * 2;
            nxt = (uint8_t *)realloc(nxt, cap);
            cur = (uint8_t *)realloc(cur, cap);
        }
        size_t pos = 0;
        for (size_t i = 0; i < pairs; ++i) {
            uint8_t x = cur[2 * i], y = cur[2 * i + 1];
            if (x == 0 && y == 0) {
                nxt[pos++] = 0; nxt[pos++] = 0; nxt[pos++] = 1; nxt[pos++] = 0;
            } else if (x == 0 && y == 1) {
                nxt[pos++] = 0; nxt[pos++] = 0; nxt[pos++] = 1;
            } else if (x == 1 && y == 0) {
                nxt[pos++] = 1; nxt[pos++] = 0; nxt[pos++] = 0;
            } else {
                nxt[pos++] = 1; nxt[pos++] = 1;
            }
        }
        if (len % 2) nxt[pos++] = cur[len - 1];
        uint8_t *t = cur; cur = nxt; nxt = t;
        len = new_len;
        ones = 0;
        for (size_t i = 0; i < len; ++i) ones += cur[i];
    }

    uint64_t cnt = 0;
    size_t p = 0;
    for (size_t i = 0; i < len; ++i) {
        if (cur[i]) {
            ++cnt;
            if (cnt == (uint64_t)n) { p = i + 1; break; }
        }
    }
    free(cur);
    free(nxt);
    return (seq_t)p;
}
