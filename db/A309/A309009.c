/*
 * A309009  Numbers that are both triangular and octahedral.
 * https://oeis.org/A309009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 4, u128 n <= 4, big any n
 * Validated: full, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A309009: numbers that are both triangular and octahedral,
   found by searching integer solutions of 3*n(n+1)=2*m(2m^2+1). */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *solutions = NULL;
static size_t sol_count = 0;

/* Find all small solutions.  The elliptic curve has only a tiny
   number of integer points; searching up to a generous bound is
   sufficient for all practical purposes. */
static void init_sequence(void)
{
    const idx_t MAX_M = 100000;            /* safe upper bound */
    seq_t *tmp = NULL;
    size_t tmp_cap = 0;
    size_t tmp_cnt = 0;

    for (idx_t m = 0; m <= MAX_M; ++m) {
        seq_t m_seq = (seq_t)m;
        seq_t val = m_seq * ((seq_t)2 * m_seq * m_seq + 1) / 3;

        seq_t t = (seq_t)8 * val + 1;
        seq_t r = floor_sqrt(t);
        if (r * r == t) {                  /* 8*val+1 is a perfect square */
            if (tmp_cnt == tmp_cap) {
                size_t new_cap = tmp_cap ? tmp_cap * 2 : 4;
                seq_t *p = mem_new(seq_t, new_cap);
                if (!p) exit(EXIT_FAILURE);
                if (tmp_cap) mem_copy(p, tmp, tmp_cap * sizeof(seq_t));
                mem_free(tmp);
                tmp = p;
                tmp_cap = new_cap;
            }
            tmp[tmp_cnt++] = val;
        }
    }

    solutions = tmp;
    sol_count = tmp_cnt;
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_sequence();
        initialized = true;
    }

    if (n == 0) return (seq_t)0;          /* undefined for offset 0 */
    if (n <= sol_count) return solutions[n - 1];
    return (seq_t)0;                      /* no further terms known */
}
