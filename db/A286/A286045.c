/*
 * A286045  Positions of 0 in A286044; complement of A003157.
 * https://oeis.org/A286045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th position (1-based) that is not a 'c' in the fixed
 *            point of a->abc, b->a, c->a (complement of A003157); computed by
 *            generating the word
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9418 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *w286045 = NULL;
static size_t w286045_len = 0, w286045_cap = 0, w286045_rd = 0;
static uint64_t *p286045 = NULL;
static size_t p286045_cnt = 0, p286045_cap = 0;

static void w286045_push(unsigned char ch) {
    if (w286045_len == w286045_cap) {
        size_t nc = w286045_cap ? w286045_cap * 2 : 1024;
        w286045 = (unsigned char *)realloc(w286045, nc);
        w286045_cap = nc;
    }
    w286045[w286045_len++] = ch;
    if (ch != 2) {
        if (p286045_cnt == p286045_cap) {
            size_t nc = p286045_cap ? p286045_cap * 2 : 256;
            p286045 = (uint64_t *)realloc(p286045, nc * sizeof(uint64_t));
            p286045_cap = nc;
        }
        p286045[p286045_cnt++] = (uint64_t)w286045_len;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (w286045_len == 0) {
        w286045_push(0);
        w286045_push(1);
        w286045_push(2);
        w286045_rd = 1;
    }
    while (p286045_cnt < n) {
        unsigned char ch = w286045[w286045_rd++];
        if (ch == 0) {
            w286045_push(0);
            w286045_push(1);
            w286045_push(2);
        } else {
            w286045_push(0);
        }
    }
    return (seq_t)p286045[n - 1];
}
