/*
 * A287359  Positions of 2 in A287356.
 * https://oeis.org/A287359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 2 in A287356, the fixed point of
 *            0->11, 1->12, 2->0 starting from 1 (computed by iterating the
 *            substitution)
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9110 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static unsigned char *A287359_buf = NULL;
static size_t A287359_len = 0;
static size_t A287359_twos = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (A287359_buf == NULL) {
        A287359_buf = (unsigned char *)malloc(1);
        A287359_buf[0] = 1;
        A287359_len = 1;
        A287359_twos = 0;
    }
    while (A287359_twos < (size_t)n) {
        size_t new_len = 0;
        for (size_t i = 0; i < A287359_len; ++i)
            new_len += (A287359_buf[i] == 2) ? 1 : 2;
        unsigned char *nb = (unsigned char *)malloc(new_len);
        size_t pos = 0, twos = 0;
        for (size_t i = 0; i < A287359_len; ++i) {
            unsigned char v = A287359_buf[i];
            if (v == 0) {
                nb[pos++] = 1; nb[pos++] = 1;
            } else if (v == 1) {
                nb[pos++] = 1; nb[pos++] = 2; twos++;
            } else {
                nb[pos++] = 0;
            }
        }
        free(A287359_buf);
        A287359_buf = nb;
        A287359_len = new_len;
        A287359_twos = twos;
    }
    size_t cnt = 0;
    for (size_t i = 0; i < A287359_len; ++i) {
        if (A287359_buf[i] == 2) {
            ++cnt;
            if (cnt == (size_t)n) return (seq_t)(i + 1);
        }
    }
    return 0;
}
