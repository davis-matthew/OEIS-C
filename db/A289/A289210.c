/*
 * A289210  Coefficients in expansion of E_6^2/E_4^3.
 * https://oeis.org/A289210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(n)=c(n)−∑_{k=1..n}b(k)·a(n−k), where c(q)=E6(q)^2 and
 *            b(q)=E4(q)^3. Here E4(q)=1+240∑_{m≥1}σ₃(m)q^m,
 *            E6(q)=1−504∑_{m≥1}σ₅(m)q^m
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 3, i64 n <= 7, i128 n <= 14,
 *            big any n
 * Helpers:   alloc_zeroed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zeroed.h"

static seq_t *a_seq   = NULL;   /* coefficients a[n] */
static seq_t *e4_seq  = NULL;
static seq_t *e6_seq  = NULL;
static seq_t *c_seq   = NULL;   /* E6^2 coefficients */
static seq_t *b1_seq  = NULL;   /* E4*E4 coefficients */
static seq_t *b_seq   = NULL;   /* E4^3 coefficients */
static size_t curr_cap = 0;     /* currently allocated length */

/* Recompute all tables up to limit (inclusive). */
static void recompute(idx_t limit)
{
    idx_t i, k, m;
    size_t sz = (size_t)limit + 1;

    /* Allocate new working arrays */
    seq_t *a   = alloc_zeroed(sz);
    seq_t *e4  = alloc_zeroed(sz);
    seq_t *e6  = alloc_zeroed(sz);
    seq_t *c   = alloc_zeroed(sz);
    seq_t *b1  = alloc_zeroed(sz);
    seq_t *b   = alloc_zeroed(sz);
    seq_t *sigma3 = alloc_zeroed(sz);
    seq_t *sigma5 = alloc_zeroed(sz);

    /* Compute divisor sums σ₃ and σ₅ (branch‑safe) */
    for (i = 1; i <= limit; ++i) {
        seq_t ii = (seq_t)i;
        seq_t d3 = ii * ii * ii;          /* i³   */
        seq_t d5 = d3 * ii * ii;          /* i⁵   */
        for (m = i; m <= limit; m += i) {
            sigma3[m] += d3;
            sigma5[m] += d5;
        }
    }

    /* E4 and E6 coefficients */
    e4[0] = 1;
    e6[0] = 1;
    for (i = 1; i <= limit; ++i) {
        e4[i] = 240 * sigma3[i];
        e6[i] = -504 * sigma5[i];
    }

    /* c = E6 * E6 (convolution) */
    c[0] = 1;
    for (i = 1; i <= limit; ++i) {
        seq_t sum = 0;
        for (k = 0; k <= i; ++k) {
            sum += e6[k] * e6[i - k];
        }
        c[i] = sum;
    }

    /* b1 = E4 * E4 (convolution) */
    b1[0] = 1;
    for (i = 1; i <= limit; ++i) {
        seq_t sum = 0;
        for (k = 0; k <= i; ++k) {
            sum += e4[k] * e4[i - k];
        }
        b1[i] = sum;
    }

    /* b = b1 * E4 (convolution) */
    for (i = 0; i <= limit; ++i) {
        seq_t sum = 0;
        for (k = 0; k <= i; ++k) {
            sum += b1[k] * e4[i - k];
        }
        b[i] = sum;
    }

    /* a = c / b  (power‑series division) */
    a[0] = 1;
    for (i = 1; i <= limit; ++i) {
        seq_t prod = 0;
        for (k = 1; k <= i; ++k) {
            prod += b[k] * a[i - k];
        }
        a[i] = c[i] - prod;
    }

    /* Free old tables */
    free(a_seq);
    free(e4_seq);
    free(e6_seq);
    free(c_seq);
    free(b1_seq);
    free(b_seq);

    /* Move new tables into static variables */
    a_seq   = a;
    e4_seq  = e4;
    e6_seq  = e6;
    c_seq   = c;
    b1_seq  = b1;
    b_seq   = b;

    /* Record capacity */
    curr_cap = sz;

    /* Clean up temporary arrays */
    free(sigma3);
    free(sigma5);
}

/* Public function */
seq_t a(idx_t n)
{
    if (n >= (idx_t)curr_cap) {
        recompute(n);
    }
    return a_seq[n];
}
