/*
 * A287358  Positions of 1 in A287356.
 * https://oeis.org/A287358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A287356, the fixed point of
 *            0->11, 1->12, 2->0 starting from 1 (computed by iterating the
 *            substitution)
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9990 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static unsigned char *a287358_s = NULL;
static idx_t a287358_len = 0;
static idx_t a287358_ones = 0;

static void a287358_expand(void) {
    idx_t newlen = 0;
    for (idx_t i = 0; i < a287358_len; ++i)
        newlen += (a287358_s[i] == 2) ? 1 : 2;
    unsigned char *t = (unsigned char *)malloc((size_t)newlen);
    idx_t p = 0, ones = 0;
    for (idx_t i = 0; i < a287358_len; ++i) {
        unsigned char v = a287358_s[i];
        if (v == 0) { t[p++] = 1; t[p++] = 1; ones += 2; }
        else if (v == 1) { t[p++] = 1; t[p++] = 2; ones += 1; }
        else { t[p++] = 0; }
    }
    free(a287358_s);
    a287358_s = t;
    a287358_len = newlen;
    a287358_ones = ones;
}

seq_t a(idx_t n) {
    if (a287358_s == NULL) {
        a287358_s = (unsigned char *)malloc(1);
        a287358_s[0] = 1;
        a287358_len = 1;
        a287358_ones = 1;
    }
    while (a287358_ones < n) a287358_expand();
    idx_t cnt = 0;
    for (idx_t i = 0; i < a287358_len; ++i) {
        if (a287358_s[i] == 1) {
            ++cnt;
            if (cnt == n) return (seq_t)(i + 1);
        }
    }
    return 0;
}
