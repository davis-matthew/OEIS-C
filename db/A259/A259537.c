/*
 * A259537  Smallest m such that A259429(m) = 2*n-1.
 * https://oeis.org/A259537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m = smallest index with A259429(m) = 2*n-1, where
 *            A259429(1)=1 and A259429(k) is the smallest unused value with
 *            (A259429(k-1)+A259429(k))/2 a cube; computed by generating
 *            A259429 (search)
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9233 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/mem.h"

static uint64_t *A259537_pos = NULL;   /* pos[v>>1] = index of odd-or-even value v (1-based), 0 if unused */
static size_t A259537_cap = 0;
static uint64_t A259537_len = 0;
static seq_t A259537_prev = 0;

static void A259537_ensure(size_t idx)
{
    if (idx < A259537_cap) return;
    size_t nc = A259537_cap ? A259537_cap : 1024;
    while (nc <= idx) nc *= 2;
    uint64_t *p = (uint64_t *)realloc(A259537_pos, nc * sizeof(uint64_t));
    if (!p) abort();
    memset(p + A259537_cap, 0, (nc - A259537_cap) * sizeof(uint64_t));
    A259537_pos = p;
    A259537_cap = nc;
}

static void A259537_step(void)
{
    if (A259537_len == 0) {
        A259537_ensure(0);
        A259537_pos[0] = 1;
        A259537_len = 1;
        A259537_prev = 1;
        return;
    }
    seq_t chosen = 0;
    for (seq_t k = 1; ; ++k) {
        seq_t twice = (k * k * k) * 2;
        if (twice <= A259537_prev) continue;
        seq_t cand = twice - A259537_prev;
        size_t ci = (size_t)(long long)(cand >> 1);
        if (ci >= A259537_cap || A259537_pos[ci] == 0) {
            chosen = cand;
            break;
        }
    }
    size_t ci = (size_t)(long long)(chosen >> 1);
    A259537_ensure(ci);
    A259537_len++;
    A259537_pos[ci] = A259537_len;
    A259537_prev = chosen;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t t = (size_t)(n - 1);   /* (2n-1)>>1 = n-1 */
    for (;;) {
        if (t < A259537_cap && A259537_pos[t] != 0)
            return (seq_t)A259537_pos[t];
        A259537_step();
    }
}
