/*
 * A317206  An alternative tribonacci representation of n: an encoding of the
 *          position of n in the A003144, A003145, A003146 table.
 * https://oeis.org/A317206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal string of letters (A=1,B=2,C=3) obtained by
 *            repeatedly writing n = A003144(k), A003145(k) or A003146(k) as
 *            the digit 1, 2 or 3 and replacing n by k until n = 1; a(0)=0,
 *            a(1)=1. Computed by building the tribonacci word a->ab, b->ac,
 *            c->a
 * Range:     u8 n <= 7, u16 n <= 24, u32 n <= 504, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9960 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static unsigned char *trw_t = NULL;
static uint32_t *trw_i = NULL;
static size_t trw_len = 0, trw_cap = 0, trw_gi = 0;
static uint32_t trw_cnt[3] = {0, 0, 0};

static void trw_push(unsigned char c) {
    if (trw_len == trw_cap) {
        size_t nc = trw_cap ? trw_cap * 2 : 1024;
        trw_t = (unsigned char *)realloc(trw_t, nc);
        trw_i = (uint32_t *)realloc(trw_i, nc * sizeof(uint32_t));
        trw_cap = nc;
    }
    trw_t[trw_len] = c;
    trw_i[trw_len] = ++trw_cnt[c];
    trw_len++;
}

static void trw_ensure(size_t n) {
    if (trw_len == 0) {
        trw_push(0);
        trw_push(1);
        trw_gi = 1;
    }
    while (trw_len < n) {
        unsigned char c = trw_t[trw_gi++];
        if (c == 0) { trw_push(0); trw_push(1); }
        else if (c == 1) { trw_push(0); trw_push(2); }
        else { trw_push(0); }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    trw_ensure((size_t)n);
    seq_t r = 0;
    idx_t m = n;
    while (m > 1) {
        unsigned char c = trw_t[m - 1];
        uint32_t k = trw_i[m - 1];
        r = r * 10 + (seq_t)(c + 1);
        m = (idx_t)k;
    }
    return r;
}
