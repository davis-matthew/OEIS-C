/*
 * A249037  Number of even terms in first n terms of A249036.
 * https://oeis.org/A249037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of even terms among A249036(1..n), where
 *            A249036(1)=1, A249036(2)=2, A249036(k) = A249036(k-1-E) +
 *            A249036(k-1-O) with E, O the counts of even and odd terms among
 *            the first k-1 terms
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A249037_v = NULL;      /* terms of A249036, 1-based */
static uint32_t *A249037_ev = NULL;  /* prefix counts of even terms */
static size_t A249037_cap = 0;
static size_t A249037_len = 0;

static void A249037_grow(size_t n)
{
    if (n <= A249037_cap) return;
    size_t nc = A249037_cap ? A249037_cap : 8;
    while (nc < n) nc <<= 1;
    A249037_v = mem_resize(seq_t, A249037_v, nc + 1);
    A249037_ev = (uint32_t *)realloc(A249037_ev, (nc + 1) * sizeof(uint32_t));
    A249037_cap = nc;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    size_t N = (size_t)n;
    A249037_grow(N < 2 ? 2 : N);
    if (A249037_len == 0) {
        A249037_v[0] = (seq_t)0;
        A249037_ev[0] = 0;
        A249037_v[1] = (seq_t)1;
        A249037_ev[1] = 0;
        A249037_v[2] = (seq_t)2;
        A249037_ev[2] = 1;
        A249037_len = 2;
    }
    while (A249037_len < N) {
        size_t nx = A249037_len + 1;
        size_t e = A249037_ev[nx - 1];
        size_t o = (nx - 1) - e;
        seq_t val = A249037_v[(nx - 1) - e] + A249037_v[(nx - 1) - o];
        A249037_v[nx] = val;
        A249037_ev[nx] = A249037_ev[nx - 1] + (((val & (seq_t)1) == (seq_t)0) ? 1u : 0u);
        A249037_len = nx;
    }
    return (seq_t)A249037_ev[N];
}
