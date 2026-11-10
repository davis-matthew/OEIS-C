/*
 * A257683  Inverse permutation to A218252.
 * https://oeis.org/A257683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index j such that A218252(j) = n (inverse permutation);
 *            computed by generating A218252 until n appears
 * Range:     u8 n <= 82, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 98 terms through n = 7545 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

static unsigned int A257683_pc(seq_t x)
{
    unsigned int c = 0;
    while (x) { c += (unsigned int)(x & 1); x >>= 1; }
    return c;
}

static uint8_t *A257683_used = NULL;
static idx_t   *A257683_pos  = NULL;
static size_t   A257683_cap  = 0;
static idx_t    A257683_cnt  = 0;
static seq_t    A257683_cur  = 0;
static size_t   A257683_nu   = 1;

static void A257683_ensure(size_t high)
{
    if (high < A257683_cap) return;
    size_t nc = A257683_cap ? A257683_cap : 64;
    while (nc <= high) nc <<= 1;
    uint8_t *u = (uint8_t *)realloc(A257683_used, nc);
    idx_t *p = (idx_t *)realloc(A257683_pos, nc * sizeof(idx_t));
    if (!u || !p) abort();
    memset(u + A257683_cap, 0, nc - A257683_cap);
    memset(p + A257683_cap, 0, (nc - A257683_cap) * sizeof(idx_t));
    A257683_used = u;
    A257683_pos = p;
    A257683_cap = nc;
}

static void A257683_mark(seq_t v)
{
    size_t i = (size_t)v;
    A257683_ensure(i);
    A257683_used[i] = 1;
    A257683_pos[i] = A257683_cnt;
}

static void A257683_step(void)
{
    if (A257683_cnt == 0) {
        A257683_cnt = 1;
        A257683_cur = 1;
        A257683_mark(A257683_cur);
        return;
    }
    seq_t m = A257683_cur;
    seq_t k = m + 1;
    seq_t res = 0;
    bool found = false;
    while (1) {
        seq_t f = k - (seq_t)A257683_pc(k);
        if (f == m) {
            A257683_ensure((size_t)k);
            if (!A257683_used[(size_t)k]) { res = k; found = true; break; }
        } else if (f > m) {
            break;
        }
        ++k;
    }
    if (!found) {
        while (1) {
            A257683_ensure(A257683_nu);
            if (!A257683_used[A257683_nu]) break;
            ++A257683_nu;
        }
        res = (seq_t)A257683_nu;
    }
    A257683_cnt++;
    A257683_cur = res;
    A257683_mark(res);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t m = (size_t)n;
    while (m >= A257683_cap || !A257683_used[m]) {
        A257683_step();
    }
    return (seq_t)A257683_pos[m];
}
