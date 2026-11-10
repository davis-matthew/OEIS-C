/*
 * A287357  Positions of 0 in A287356.
 * https://oeis.org/A287357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in the fixed point of 0->11,
 *            1->12, 2->0 starting from 1 (A287356); computed by expanding the
 *            substitution and scanning
 * Range:     u8 n <= 45, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9826 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A287357_buf = NULL;
static uint64_t A287357_len = 0;
static uint64_t A287357_zeros = 0;

static void A287357_expand(void) {
    uint64_t newlen = 0;
    for (uint64_t i = 0; i < A287357_len; ++i)
        newlen += (A287357_buf[i] == 2) ? 1 : 2;
    unsigned char *nb = (unsigned char *)malloc((size_t)newlen);
    uint64_t pos = 0, z = 0;
    for (uint64_t i = 0; i < A287357_len; ++i) {
        unsigned char v = A287357_buf[i];
        if (v == 0) { nb[pos++] = 1; nb[pos++] = 1; }
        else if (v == 1) { nb[pos++] = 1; nb[pos++] = 2; }
        else { nb[pos++] = 0; z++; }
    }
    free(A287357_buf);
    A287357_buf = nb;
    A287357_len = newlen;
    A287357_zeros = z;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (A287357_buf == NULL) {
        A287357_buf = (unsigned char *)malloc(1);
        A287357_buf[0] = 1;
        A287357_len = 1;
        A287357_zeros = 0;
    }
    while (A287357_zeros < (uint64_t)n)
        A287357_expand();
    uint64_t cnt = 0;
    for (uint64_t i = 0; i < A287357_len; ++i) {
        if (A287357_buf[i] == 0) {
            if (++cnt == (uint64_t)n) return (seq_t)(i + 1);
        }
    }
    return 0;
}
