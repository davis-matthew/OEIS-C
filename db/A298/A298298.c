/*
 * A298298  Numbers k such that b(k+1) = b(k) + 2, where b = A298296.
 * https://oeis.org/A298298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   b = A298296 (b(0)=3, b(1)=4, b(2)=5; b(n) = least positive
 *            integer not in the sequence a(0)=1, a(1)=2, a(n)=b(n)+2*b(n-1)).
 *            Here a(n) is the n-th k (k >= 1) with b(k) = b(k-1)+2 in
 *            0-indexed b, which is the same as b(k+1) = b(k)+2 in the
 *            1-indexed form. Computed by a sieve-like generation
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static uint8_t *A298298_bits = NULL;
static size_t A298298_nbytes = 0;
static seq_t *A298298_res = NULL;
static size_t A298298_cnt = 0, A298298_cap = 0;
static uint64_t A298298_cand = 6;
static uint64_t A298298_i = 3;
static uint64_t A298298_prev = 5;
static int A298298_init = 0;

static void A298298_mark(uint64_t v)
{
    size_t need = (size_t)(v >> 3) + 1;
    if (need > A298298_nbytes) {
        size_t nb = A298298_nbytes ? A298298_nbytes : 64;
        while (nb < need) nb <<= 1;
        uint8_t *t = (uint8_t *)realloc(A298298_bits, nb);
        if (!t) abort();
        for (size_t j = A298298_nbytes; j < nb; ++j) t[j] = 0;
        A298298_bits = t;
        A298298_nbytes = nb;
    }
    A298298_bits[v >> 3] |= (uint8_t)(1U << (v & 7));
}

static int A298298_used(uint64_t v)
{
    if ((size_t)(v >> 3) >= A298298_nbytes) return 0;
    return (A298298_bits[v >> 3] >> (v & 7)) & 1U;
}

static void A298298_push(uint64_t k)
{
    if (A298298_cnt >= A298298_cap) {
        size_t nc = A298298_cap ? A298298_cap * 2 : 64;
        seq_t *t = mem_resize(seq_t, A298298_res, nc);
        if (!t) abort();
        A298298_res = t;
        A298298_cap = nc;
    }
    A298298_res[A298298_cnt++] = (seq_t)k;
}

seq_t a(idx_t n)
{
    if (!A298298_init) {
        A298298_init = 1;
        for (uint64_t v = 1; v <= 5; ++v) A298298_mark(v);
        A298298_mark(13);
    }
    while (A298298_cnt < (size_t)n) {
        while (A298298_used(A298298_cand)) ++A298298_cand;
        uint64_t b = A298298_cand++;
        A298298_mark(b);
        A298298_mark(b + 2 * A298298_prev);
        if (b == A298298_prev + 2) A298298_push(A298298_i);
        A298298_prev = b;
        ++A298298_i;
    }
    return A298298_res[(size_t)n - 1];
}
