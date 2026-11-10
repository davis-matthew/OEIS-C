/*
 * A285138  Positions of 1 in A285136; complement of A285137.
 * https://oeis.org/A285138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A285136, the fixed point of f =
 *            morph^2 (0 -> 000110, 1 -> 1010100001); found by building the
 *            word
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9649 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t N = (size_t)n;
    size_t cap = 3 * N + 64;
    uint8_t *w = (uint8_t *)malloc(cap + 16);
    if (!w) return (seq_t)0;
    static const uint8_t f0[6] = {0, 0, 0, 1, 1, 0};
    static const uint8_t f1[10] = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1};
    size_t len = 0;
    for (int k = 0; k < 10; ++k) w[len++] = f1[k];   /* f(w[0]) with w[0] = 1 */
    for (size_t i = 1; len < cap && i < len; ++i) {
        if (w[i]) {
            for (int k = 0; k < 10; ++k) w[len++] = f1[k];
        } else {
            for (int k = 0; k < 6; ++k) w[len++] = f0[k];
        }
    }
    size_t cnt = 0, pos = 0;
    for (size_t i = 0; i < len; ++i) {
        if (w[i]) {
            ++cnt;
            if (cnt == N) { pos = i + 1; break; }
        }
    }
    free(w);
    return (seq_t)pos;
}
