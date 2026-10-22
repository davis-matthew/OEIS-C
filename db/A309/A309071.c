/*
 * A309071  Complete list of solutions to y^2 = x^3 + 20*x; sequence gives x
 *          values.
 * https://oeis.org/A309071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 4, u128 n <= 4, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A309071: compute x-values for integer solutions of y^2 = x^3 + 20*x.
   The solutions are obtained by enumerating i such that
   (1) i^4 + 20 is a perfect square → x = i^2
   (2) 5*i^4 + 4 is a perfect square → x = 5*i^2.
   Only finitely many solutions exist (0, 4, 5, 720).  The array is
   built once and sorted.  a(n) returns the n-th element (1‑based),
   or 0 for n beyond the number of solutions. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

static seq_t solutions[16];
static size_t sol_cnt = 0;

/* Comparator for qsort on seq_t values. */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t v1 = *(const seq_t *)p1;
    const seq_t v2 = *(const seq_t *)p2;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

static inline void add_solution(seq_t val)
{
    for (size_t i = 0; i < sol_cnt; ++i)
        if (solutions[i] == val)
            return;                      /* avoid duplicates */
    if (sol_cnt < sizeof(solutions) / sizeof(solutions[0]))
        solutions[sol_cnt++] = val;
}

static void build_solutions(void)
{
    if (sol_cnt) return;                  /* already built */

    /* 1. Square solutions: x = i^2 where i^4 + 20 is a square */
    const uint64_t MAX_I_SQ = 200;         /* covers all solutions (i=0,2) */
    for (uint64_t i = 0; i <= MAX_I_SQ; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t i2 = i_seq * i_seq;          /* i^2          */
        seq_t i4 = i2 * i2;                /* i^4          */
        seq_t val = i4 + (seq_t)20;
        seq_t s = floor_sqrt(val);
        if (s * s == val)
            add_solution(i2);              /* x = i^2      */
    }

    /* 2. j = 5 case: x = 5*i^2 where 5*i^4 + 4 is a square */
    const uint64_t MAX_I_J5 = 2000;        /* covers all solutions (i=0,1,12) */
    for (uint64_t i = 0; i <= MAX_I_J5; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t i2   = i_seq * i_seq;            /* i^2   */
        seq_t i4   = i2 * i2;                  /* i^4   */
        seq_t lhs  = (seq_t)5 * i4 + (seq_t)4;
        seq_t k    = floor_sqrt(lhs);
        if (k * k == lhs) {
            seq_t x = i2 * (seq_t)5;          /* x = 5*i^2 */
            add_solution(x);
        }
    }

    /* Sort the solutions */
    if (sol_cnt > 1)
        qsort(solutions, sol_cnt, sizeof(seq_t), cmp_seq_t);
}

seq_t a(idx_t n)
{
    build_solutions();
    if (n == 0 || n > sol_cnt) return 0;
    return solutions[n - 1];
}
