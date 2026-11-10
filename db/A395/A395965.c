/*
 * A395965  Numbers k such that A395964(k) = 0.
 * https://oeis.org/A395965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 0 with A395964(k) = 0, where A395964 is the
 *            second Thue-Morse transform iterate of the Fibonacci word
 *            (computed by direct generation, doubling length until enough
 *            zeros)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static void A395965_tm(const uint8_t *in, uint8_t *out, uint64_t len)
{
    if (len == 0) return;
    out[0] = 0;
    uint64_t zc = 1, oc = 0;
    for (uint64_t i = 1; i < len; ++i) {
        if (in[i] == 0) { out[i] = out[zc]; ++zc; }
        else { out[i] = (uint8_t)(1 - out[oc]); ++oc; }
    }
}

static uint64_t *A395965_pos = NULL;
static uint64_t A395965_cnt = 0;
static uint64_t A395965_len = 0;

static void A395965_build(uint64_t L)
{
    uint8_t *cur = (uint8_t *)malloc(1);
    cur[0] = 0;
    uint64_t cl = 1;
    while (cl < L) {
        uint64_t zeros = 0;
        for (uint64_t i = 0; i < cl; ++i) if (cur[i] == 0) ++zeros;
        uint64_t nl = cl + zeros;
        uint8_t *nx = (uint8_t *)malloc(nl);
        uint64_t p = 0;
        for (uint64_t i = 0; i < cl; ++i) {
            if (cur[i] == 0) { nx[p++] = 0; nx[p++] = 1; }
            else nx[p++] = 0;
        }
        free(cur);
        cur = nx;
        cl = nl;
    }
    uint8_t *w1 = (uint8_t *)malloc(L);
    uint8_t *w2 = (uint8_t *)malloc(L);
    A395965_tm(cur, w1, L);
    A395965_tm(w1, w2, L);
    free(A395965_pos);
    A395965_pos = (uint64_t *)malloc(L * sizeof(uint64_t));
    uint64_t c = 0;
    for (uint64_t i = 0; i < L; ++i)
        if (w2[i] == 0) A395965_pos[c++] = i;
    A395965_cnt = c;
    A395965_len = L;
    free(cur); free(w1); free(w2);
}

seq_t a(idx_t n)
{
    uint64_t L = A395965_len ? A395965_len : 256;
    while (A395965_cnt < (uint64_t)n || A395965_len == 0) {
        if (A395965_len != 0) L = A395965_len * 2;
        A395965_build(L);
    }
    return (seq_t)A395965_pos[n - 1];
}
