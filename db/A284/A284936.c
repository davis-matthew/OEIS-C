/*
 * A284936  Positions of 0 in A284935; complement of A284937.
 * https://oeis.org/A284936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A284935, the fixed point of the
 *            morphism 0 -> 01, 1 -> 1100. The word is built in place: start
 *            with "01", the image of the first letter, then append the image
 *            of each later letter. Then scan the word for its zeros
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9557 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t cnt = (size_t)n;
    size_t cap = 4 * cnt + 16;
    uint8_t *buf = (uint8_t *)malloc(cap + 8);
    if (!buf) return 0;
    size_t len = 2;
    buf[0] = 0;
    buf[1] = 1;
    for (size_t i = 1; len < cap; i++) {
        if (buf[i] == 0) {
            buf[len++] = 0;
            buf[len++] = 1;
        } else {
            buf[len++] = 1;
            buf[len++] = 1;
            buf[len++] = 0;
            buf[len++] = 0;
        }
    }
    size_t zeros = 0, pos = 0;
    for (size_t i = 0; i < len; i++) {
        if (buf[i] == 0) {
            zeros++;
            if (zeros == cnt) { pos = i + 1; break; }
        }
    }
    free(buf);
    return (seq_t)pos;
}
