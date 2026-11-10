/*
 * A284393  Positions of 1 in A284391; complement of A284392.
 * https://oeis.org/A284393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A284391 (1-limiting word of
 *            0->1, 1->001); computed by iterating the morphism
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9797 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static unsigned char *A284393_expand(const unsigned char *w, size_t len, size_t *newlen)
{
    size_t cap = len * 3 + 1;
    unsigned char *r = (unsigned char *)malloc(cap);
    size_t pos = 0, i;
    if (!r) abort();
    for (i = 0; i < len; ++i) {
        if (w[i] == 0) {
            r[pos++] = 1;
        } else {
            r[pos++] = 0;
            r[pos++] = 0;
            r[pos++] = 1;
        }
    }
    *newlen = pos;
    return r;
}

seq_t a(idx_t n)
{
    size_t len = 1, i;
    unsigned char *w = (unsigned char *)malloc(1);
    unsigned long long target = (unsigned long long)n;
    unsigned long long ones;
    if (!w) abort();
    w[0] = 1;                     /* iteration 1 from "0" */
    ones = 1;
    while (ones < target) {
        size_t l2;
        unsigned char *t = A284393_expand(w, len, &l2);
        size_t l3;
        unsigned char *u = A284393_expand(t, l2, &l3);
        free(t);
        free(w);
        w = u;
        len = l3;
        ones = 0;
        for (i = 0; i < len; ++i) ones += w[i];
    }
    {
        unsigned long long c = 0;
        size_t p = 0;
        for (i = 0; i < len; ++i) {
            if (w[i]) {
                if (++c == target) { p = i + 1; break; }
            }
        }
        free(w);
        return (seq_t)p;
    }
}
