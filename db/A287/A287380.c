/*
 * A287380  Positions of 1 in A287931; complement of A287379.
 * https://oeis.org/A287380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A287931, the fixed point of
 *            00->0010, 1->10 starting with 00; computed by generating the
 *            word and scanning
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9278 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static unsigned char *A287380_buf = NULL;
static size_t A287380_len = 0;

static void A287380_expand(void) {
    size_t cap = A287380_len * 2 + 10;
    unsigned char *nb = (unsigned char *)malloc(cap);
    size_t i = 0, nl = 0;
    if (!nb) return;
    while (i < A287380_len) {
        if (A287380_buf[i] == 1) {
            nb[nl++] = 1; nb[nl++] = 0; i++;
        } else if (i + 1 < A287380_len && A287380_buf[i + 1] == 0) {
            nb[nl++] = 0; nb[nl++] = 0; nb[nl++] = 1; nb[nl++] = 0; i += 2;
        } else {
            nb[nl++] = 0; i++;
        }
    }
    free(A287380_buf);
    A287380_buf = nb;
    A287380_len = nl;
}

seq_t a(idx_t n) {
    size_t ones, i;
    if (n == 0) return 0;
    if (A287380_len == 0) {
        A287380_buf = (unsigned char *)malloc(2);
        if (!A287380_buf) return 0;
        A287380_buf[0] = 0; A287380_buf[1] = 0;
        A287380_len = 2;
    }
    for (;;) {
        ones = 0;
        for (i = 0; i < A287380_len; i++) ones += A287380_buf[i];
        if (ones >= (size_t)n + 2) break;
        size_t before = A287380_len;
        A287380_expand();
        if (A287380_len == before) return 0;
    }
    ones = 0;
    for (i = 0; i < A287380_len; i++) {
        if (A287380_buf[i]) {
            ones++;
            if ((idx_t)ones == n) return (seq_t)(i + 1);
        }
    }
    return 0;
}
