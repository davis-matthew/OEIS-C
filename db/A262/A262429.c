/*
 * A262429  Inverse permutation to A262411.
 * https://oeis.org/A262429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that A262411(k) = n; A262411 (lexicographically
 *            earliest permutation with consecutive ternary representations
 *            overlapping) is generated greedily and inverted by search
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9035 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static void A262429_digits(uint64_t x, int *buf, int *len)
{
    int tmp[64];
    int t = 0;
    if (x == 0) { buf[0] = 0; *len = 1; return; }
    while (x > 0) { tmp[t++] = (int)(x % 3); x /= 3; }
    for (int i = 0; i < t; ++i) buf[i] = tmp[t - 1 - i];
    *len = t;
}

static bool A262429_overlap(uint64_t a, uint64_t b)
{
    int da[64], db[64], la = 0, lb = 0;
    A262429_digits(a, da, &la);
    A262429_digits(b, db, &lb);
    int m = la < lb ? la : lb;
    for (int k = 1; k <= m; ++k) {
        bool ok = true;
        for (int i = 0; i < k; ++i)
            if (da[la - k + i] != db[i]) { ok = false; break; }
        if (ok) return true;
    }
    for (int k = 1; k <= m; ++k) {
        bool ok = true;
        for (int i = 0; i < k; ++i)
            if (db[lb - k + i] != da[i]) { ok = false; break; }
        if (ok) return true;
    }
    return false;
}

static uint64_t *A262429_pos = NULL;   /* pos[x] = index (1-based) of x, 0 if unseen */
static uint64_t A262429_cap = 0;
static uint64_t A262429_len = 0;
static uint64_t A262429_prev = 0;
static uint64_t A262429_first = 1;     /* smallest possibly unused value */

static void A262429_ensure(uint64_t idx)
{
    if (idx < A262429_cap) return;
    uint64_t nc = A262429_cap ? A262429_cap * 2 : 64;
    while (nc <= idx) nc *= 2;
    uint64_t *np = (uint64_t *)realloc(A262429_pos, nc * sizeof(uint64_t));
    if (!np) abort();
    for (uint64_t i = A262429_cap; i < nc; ++i) np[i] = 0;
    A262429_pos = np;
    A262429_cap = nc;
}

static void A262429_step(void)
{
    if (A262429_len == 0) {
        A262429_ensure(1);
        A262429_pos[1] = 1;
        A262429_len = 1;
        A262429_prev = 1;
        A262429_first = 2;
        return;
    }
    A262429_ensure(A262429_first);
    while (A262429_pos[A262429_first]) {
        ++A262429_first;
        A262429_ensure(A262429_first);
    }
    uint64_t c = A262429_first;
    for (;;) {
        A262429_ensure(c);
        if (!A262429_pos[c] && A262429_overlap(A262429_prev, c)) break;
        ++c;
    }
    ++A262429_len;
    A262429_pos[c] = A262429_len;
    A262429_prev = c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t v = (uint64_t)n;
    for (;;) {
        if (v < A262429_cap && A262429_pos[v]) break;
        A262429_step();
    }
    return (seq_t)A262429_pos[v];
}
