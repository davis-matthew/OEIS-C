/*
 * A258743  Fixed points in A055266.
 * https://oeis.org/A258743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number k such that A055266(k) = k; computed by
 *            generating the greedy sequence A055266 (smallest unused positive
 *            integer whose sum with the previous term is not prime) and
 *            recording fixed points
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

static uint64_t *fp_seq = NULL;
static size_t fp_len = 0, fp_cap = 0;

static unsigned char *fp_used = NULL;
static size_t fp_used_cap = 0;
static uint64_t fp_low = 1;

static uint64_t *fp_fixed = NULL;
static size_t fp_fixed_len = 0, fp_fixed_cap = 0;

static void fp_ensure_used(size_t idx)
{
    if (idx < fp_used_cap) return;
    size_t nc = fp_used_cap ? fp_used_cap * 2 : 1024;
    while (nc <= idx) nc *= 2;
    unsigned char *t = (unsigned char *)realloc(fp_used, nc);
    if (!t) abort();
    memset(t + fp_used_cap, 0, nc - fp_used_cap);
    fp_used = t;
    fp_used_cap = nc;
}

static void fp_push_seq(uint64_t v)
{
    if (fp_len >= fp_cap) {
        size_t nc = fp_cap ? fp_cap * 2 : 1024;
        uint64_t *t = (uint64_t *)realloc(fp_seq, nc * sizeof(uint64_t));
        if (!t) abort();
        fp_seq = t;
        fp_cap = nc;
    }
    fp_seq[fp_len++] = v;
}

static void fp_push_fixed(uint64_t v)
{
    if (fp_fixed_len >= fp_fixed_cap) {
        size_t nc = fp_fixed_cap ? fp_fixed_cap * 2 : 64;
        uint64_t *t = (uint64_t *)realloc(fp_fixed, nc * sizeof(uint64_t));
        if (!t) abort();
        fp_fixed = t;
        fp_fixed_cap = nc;
    }
    fp_fixed[fp_fixed_len++] = v;
}

static void fp_step(void)
{
    uint64_t last = fp_len ? fp_seq[fp_len - 1] : 0;
    fp_ensure_used((size_t)fp_low);
    while (fp_used[fp_low]) {
        ++fp_low;
        fp_ensure_used((size_t)fp_low);
    }
    uint64_t c = fp_low;
    for (;;) {
        fp_ensure_used((size_t)c);
        if (!fp_used[c] && (last == 0 || !is_prime_u64(last + c))) break;
        ++c;
    }
    fp_used[c] = 1;
    fp_push_seq(c);
    if (c == (uint64_t)fp_len) fp_push_fixed(c);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while ((uint64_t)fp_fixed_len < (uint64_t)n) fp_step();
    return (seq_t)fp_fixed[n - 1];
}
