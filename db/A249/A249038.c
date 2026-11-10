/*
 * A249038  Number of odd terms in first n terms of A249036.
 * https://oeis.org/A249038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd terms among the first n terms of A249036,
 *            where A249036: b(1)=1, b(2)=2, b(n) = b(n-1-E) + b(n-1-O) for
 *            n>2, E,O = number of even/odd terms among b(1..n-1)
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

static seq_t   *A249038_t  = NULL;
static uint32_t *A249038_ev = NULL;
static size_t A249038_cap = 0;
static size_t A249038_len = 0;

static void A249038_ensure(size_t n)
{
    if (n <= A249038_cap) return;
    size_t nc = A249038_cap ? A249038_cap << 1 : 8;
    while (nc < n) nc <<= 1;
    A249038_t  = mem_resize(seq_t, A249038_t, nc + 1);
    A249038_ev = (uint32_t *)realloc(A249038_ev, (nc + 1) * sizeof(uint32_t));
    A249038_cap = nc;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    size_t N = (size_t)n;
    A249038_ensure(N < 2 ? 2 : N);
    if (A249038_len == 0) {
        A249038_t[0] = (seq_t)0;
        A249038_ev[0] = 0;
        A249038_t[1] = (seq_t)1;
        A249038_ev[1] = 0;
        A249038_t[2] = (seq_t)2;
        A249038_ev[2] = 1;
        A249038_len = 2;
    }
    while (A249038_len < N) {
        size_t nx = A249038_len + 1;
        size_t ev = A249038_ev[nx - 1];
        size_t od = (nx - 1) - ev;
        seq_t val = A249038_t[(nx - 1) - ev] + A249038_t[(nx - 1) - od];
        A249038_t[nx] = val;
        A249038_ev[nx] = A249038_ev[nx - 1] + (((val & (seq_t)1) == (seq_t)0) ? 1 : 0);
        A249038_len = nx;
    }
    return (seq_t)(N - A249038_ev[N]);
}
