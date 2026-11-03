/*
 * A379147  Irregular triangle T(n, k), n >= 0, k = 1..2^A007895(n), read by
 *          rows; the n-th row lists the integers m such that A184617(abs(m))
 *          = A003714(n).
 * https://oeis.org/A379147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, base
 * Formula:   a(n) generates the irregular triangle T(n,k) as defined in OEIS
 *            A379147. The function computes the n-th term by locating the row
 *            that contains index n, generating the list of signed sums for
 *            that row, sorting them, and returning the value at the required
 *            position
 * Range:     i8 n <= 171, i16 n <= 10922, i32 n <= 10922, i64 n <= 10922,
 *            i128 n <= 10922, big any n
 * Validated: standard, pass, 73 terms through n = 9804 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* comparison function for qsort on seq_t */
static int cmp_seq_t_const(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return  1;
    return 0;
}

/* global storage for Fibonacci numbers F(2+i) (i ≥ 0) */
static idx_t *g_fibs     = NULL;   /* Fibonacci values */
static size_t g_fibs_len = 0;      /* number of elements stored */
static size_t g_fibs_cap = 0;      /* allocated capacity */

/* ensure that g_fibs contains Fibonacci numbers up to > max_val */
static void ensure_fibs_up_to(idx_t max_val)
{
    if (g_fibs_len == 0) {
        g_fibs_cap = 16;
        g_fibs = (idx_t *)malloc(g_fibs_cap * sizeof(idx_t));
        g_fibs[0] = (idx_t)1;          /* F(2) */
        g_fibs[1] = (idx_t)2;          /* F(3) */
        g_fibs_len = 2;
    }
    while (g_fibs_len < g_fibs_cap &&
           g_fibs[g_fibs_len - 1] <= max_val) {
        /* grow if necessary */
        if (g_fibs_len == g_fibs_cap) {
            g_fibs_cap *= 2;
            g_fibs = (idx_t *)realloc(g_fibs, g_fibs_cap * sizeof(idx_t));
        }
        g_fibs[g_fibs_len] = g_fibs[g_fibs_len - 2] + g_fibs[g_fibs_len - 1];
        g_fibs_len++;
    }
}

/* compute Zeckendorf exponents for n.
   exps_out must have space for at least 90 ints.  The function
   writes the exponents (0‑based) into exps_out and sets *k_out.   */
static void zeckendorf_exps(idx_t n, int *exps_out, size_t *k_out)
{
    if (n == 0) {
        *k_out = 0;
        return;
    }
    size_t count = 0;
    idx_t rem = n;
    while (rem > 0) {
        /* find largest Fibonacci <= rem */
        size_t i = 0;
        while (i < g_fibs_len && g_fibs[i] <= rem)
            i++;
        /* i points to first > rem, so i-1 is largest <= rem */
        i--;
        exps_out[count++] = (int)i;
        rem -= g_fibs[i];
    }
    *k_out = count;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* 1. Pre‑compute Fibonacci numbers up to n */
    ensure_fibs_up_to(n);

    /* 2. locate the row that contains index n */
    size_t cumulative = 0;
    idx_t r = 0;                 /* current row index */
    for (;;) {
        int exps[90];
        size_t k;
        zeckendorf_exps(r, exps, &k);
        size_t row_len = (size_t)1 << k;            /* 2^k */
        if (cumulative + row_len > (size_t)n)
            break;
        cumulative += row_len;
        r++;
    }
    size_t pos = (size_t)n - cumulative;           /* position in the row (0‑based) */

    /* 3. recompute exponents for the chosen row */
    int exps[90];
    size_t k;
    zeckendorf_exps(r, exps, &k);

    /* 4. compute total sum S_total = Σ 2^exponent */
    seq_t S_total = 0;
    for (size_t j = 0; j < k; ++j)
        S_total += (seq_t)((unsigned long long)1 << exps[j]);

    /* 5. generate all subset sums (subset of exponents)
       and sort them */
    size_t subset_cnt = (size_t)1 << k;             /* number of subsets */
    seq_t *subset_sums = mem_new(seq_t, subset_cnt);
    for (size_t mask = 0; mask < subset_cnt; ++mask) {
        seq_t sum = 0;
        for (size_t j = 0; j < k; ++j) {
            if (mask & ((size_t)1 << j))
                sum += (seq_t)((unsigned long long)1 << exps[j]);
        }
        subset_sums[mask] = sum;
    }
    mem_qsort(subset_sums, subset_cnt, sizeof(seq_t), cmp_seq_t_const);

    /* 6. convert position to subset sum */
    seq_t subset = subset_sums[pos];
    mem_free(subset_sums);

    /* 7. compute the final value */
    seq_t result = -S_total + (seq_t)2 * subset;
    return result;
}
