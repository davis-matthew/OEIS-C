/*
 * A284676  Positions of 1 in A284674; complement of A284675.
 * https://oeis.org/A284676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in the word w = lim sigma^2(0),
 *            with sigma: 0->1, 1->0111 (w is the fixed point of 0->0111,
 *            1->1011101110111, built as a prefix; a(n) = index of n-th 1,
 *            1-based)
 * Range:     u8 n <= 195, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9080 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    size_t cap = (size_t)n * 2 + 32;
    uint8_t *w = (uint8_t *)malloc(cap + 32);
    if (!w) return (seq_t)0;
    size_t written = 0, i = 0;
    w[0] = 0; w[1] = 1; w[2] = 1; w[3] = 1;
    while (written < cap) {
        if (w[i] == 0) {
            w[written++] = 0; w[written++] = 1; w[written++] = 1; w[written++] = 1;
        } else {
            w[written++] = 1;
            for (int k = 0; k < 3; k++) {
                w[written++] = 0; w[written++] = 1; w[written++] = 1; w[written++] = 1;
            }
        }
        i++;
    }
    size_t cnt = 0, pos = 0;
    for (size_t j = 0; j < written; j++) {
        if (w[j] == 1) {
            cnt++;
            if (cnt == (size_t)n) { pos = j + 1; break; }
        }
    }
    free(w);
    return (seq_t)pos;
}
