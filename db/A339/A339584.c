/*
 * A339584  A ternary sequence: a(n) = 1 if n is in A003156, 2 if n is in
 *          A003157, 3 if n is in A003158.
 * https://oeis.org/A339584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A092606(n) + 1, where A092606 is the fixed point of the
 *            morphism 0 -> 021, 1 -> 0, 2 -> 0 starting from 0 (built by
 *            self-expansion)
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Validated: partial, pass, 94 terms through n = 94 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static unsigned char *t339584_w = NULL;
static size_t t339584_len = 0, t339584_cap = 0, t339584_rd = 0;

static void t339584_push(unsigned char c) {
    if (t339584_len == t339584_cap) {
        size_t nc = t339584_cap ? t339584_cap * 2 : 1024;
        unsigned char *p = (unsigned char *)realloc(t339584_w, nc);
        if (!p) exit(1);
        t339584_w = p;
        t339584_cap = nc;
    }
    t339584_w[t339584_len++] = c;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (t339584_len == 0) {
        t339584_push(0);
        t339584_push(2);
        t339584_push(1);
        t339584_rd = 1;
    }
    while ((idx_t)t339584_len < n) {
        unsigned char c = t339584_w[t339584_rd++];
        if (c == 0) {
            t339584_push(0);
            t339584_push(2);
            t339584_push(1);
        } else {
            t339584_push(0);
        }
    }
    return (seq_t)t339584_w[n - 1] + 1;
}
