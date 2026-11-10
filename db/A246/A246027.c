/*
 * A246027  a(n) = n - A071049(n).
 * https://oeis.org/A246027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - A071049(n), where A071049(n) is the number of 1's in
 *            generation n of Rule 110 started from a single 1 (computed by
 *            direct simulation)
 * Range:     i8 n <= 301, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 10000 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"

static const uint8_t A246027_rule[8] = {0,1,1,1,0,1,1,0};

seq_t a(idx_t n) {
    static seq_t *cnt = NULL;
    static size_t ccap = 0;
    static uint8_t *row = NULL;
    static size_t rcap = 0;
    static idx_t cur = 0;

    if (cnt == NULL) {
        ccap = 64;
        cnt = mem_new(seq_t, ccap);
        cnt[0] = (seq_t)1;
        rcap = 64;
        row = (uint8_t*)calloc(rcap, 1);
        row[0] = 1;
        cur = 0;
    }

    while (cur < n) {
        size_t k = (size_t)cur;
        size_t next_len = 2 * k + 3;
        if (next_len > rcap) {
            size_t nc = rcap * 2;
            while (nc < next_len) nc *= 2;
            uint8_t *t = (uint8_t*)realloc(row, nc);
            memset(t + rcap, 0, nc - rcap);
            row = t;
            rcap = nc;
        }
        if (k + 2 > ccap) {
            size_t nc = ccap * 2;
            cnt = mem_resize(seq_t, cnt, nc);
            ccap = nc;
        }
        size_t c = 0;
        for (size_t j = next_len; j-- > 0; ) {
            uint8_t L = (j >= 2) ? row[j - 2] : 0;
            uint8_t C = (j >= 1) ? row[j - 1] : 0;
            uint8_t R = row[j];
            uint8_t v = A246027_rule[(L << 2) | (C << 1) | R];
            row[j] = v;
            c += v;
        }
        cur++;
        cnt[k + 1] = (seq_t)c;
    }
    return (seq_t)n - cnt[(size_t)n];
}
