/*
 * A352703  G.f. A(x) satisfies: A(x)^5 = A(x^5) + 5*x.
 * https://oeis.org/A352703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   A(x) satisfies A(x)^5 = A(x^5) + 5*x
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 15, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1; for n≥2,
 *   let S(n)=∑_{i1+⋯+i5=n, each ij<n} a(i1)…a(i5)
 *   if n%5==0: a(n) = (a(n/5)-S(n))/5
 *   else     : a(n) = -S(n)/5
 * S(n) is the coefficient at x^n of the 5‑fold convolution of
 * the series without the term a(n).  The convolution is updated
 * efficiently by binomial expansion of (P + anxⁿ)⁵, where P is
 * the polynomial containing a(0)…a(n‑1).  The algorithm runs in
 * O(N²) time with very small constants and uses only integer
 * arithmetic.  All intermediate values fit into seq_t provided
 * that seq_t is 128‑bit or an arbitrary‑precision type. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The following types are supplied by the build environment */
typedef idx_t idx_t;
typedef seq_t seq_t;

static idx_t current_max = 0;          /* number of values already known (indices 0..current_max-1) */
static idx_t current_cap  = 0;          /* allocated array capacity */
static bool   initialized   = false;

static seq_t *a_arr = NULL;   /* a[(idx)]  */
static seq_t *c2    = NULL;   /* 2‑fold convolution of a   */
static seq_t *c3    = NULL;   /* 3‑fold convolution of a   */
static seq_t *c4    = NULL;   /* 4‑fold convolution of a   */
static seq_t *c5    = NULL;   /* 5‑fold convolution of a   */

/* Ensure that the arrays can hold indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n < current_cap)
        return;

    idx_t new_cap = current_cap ? current_cap << 1 : 64;
    while (new_cap <= n) new_cap <<= 1;

    a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    c2   = (seq_t *)realloc(c2,   new_cap * sizeof(seq_t));
    c3   = (seq_t *)realloc(c3,   new_cap * sizeof(seq_t));
    c4   = (seq_t *)realloc(c4,   new_cap * sizeof(seq_t));
    c5   = (seq_t *)realloc(c5,   new_cap * sizeof(seq_t));

    /* zero the newly allocated part */
    size_t old_cap = current_cap;
    for (size_t i = old_cap; i < (size_t)new_cap; ++i) {
        a_arr[i] = 0;
        c2[i]    = 0;
        c3[i]    = 0;
        c4[i]    = 0;
        c5[i]    = 0;
    }
    current_cap = new_cap;
}

/* Extend the sequence up to index n (inclusive) */
static void compute_up_to(idx_t n)
{
    ensure_capacity(n);
    if (!initialized) {           /* first time: set initial values */
        a_arr[0] = 1;
        c2[0]    = 1;
        c3[0]    = 1;
        c4[0]    = 1;
        c5[0]    = 1;
        current_max = 1;          /* 0 has been generated */
        initialized = true;
    }

    for (idx_t i = current_max; i <= n; ++i) {
        seq_t an;
        if (i == 0) {
            an = 1;
        } else if (i == 1) {
            an = 1;
        } else {
            seq_t S = c5[i];            /* 5‑fold convolution before adding a[i] */
            if (i % 5 == 0) {
                an = (a_arr[i / 5] - S) / 5;
            } else {
                an = -(S) / 5;
            }
        }

        /* pre‑compute powers of an */
        seq_t p2 = an * an;
        seq_t p3 = p2 * an;
        seq_t p4 = p3 * an;
        seq_t p5 = p4 * an;

        /* Update the 5‑fold convolution (c5) */
        for (idx_t d = i; d < current_cap; ++d) {
            if (d >= i)          c5[d] += 5 * an * c4[d - i];
            if (d >= 2 * i)      c5[d] += 10 * p2 * c3[d - 2 * i];
            if (d >= 3 * i)      c5[d] += 10 * p3 * c2[d - 3 * i];
            if (d >= 4 * i)      c5[d] += 5 * p4 * a_arr[d - 4 * i];
            if (d == 5 * i)      c5[d] += p5;
        }

        /* Update the 4‑fold convolution (c4) */
        for (idx_t d = i; d < current_cap; ++d) {
            if (d >= i)          c4[d] += 4 * an * c3[d - i];
            if (d >= 2 * i)      c4[d] += 6 * p2 * c2[d - 2 * i];
            if (d >= 3 * i)      c4[d] += 4 * p3 * a_arr[d - 3 * i];
            if (d == 4 * i)      c4[d] += p4;
        }

        /* Update the 3‑fold convolution (c3) */
        for (idx_t d = i; d < current_cap; ++d) {
            if (d >= i)          c3[d] += 3 * an * c2[d - i];
            if (d >= 2 * i)      c3[d] += 3 * p2 * a_arr[d - 2 * i];
            if (d == 3 * i)      c3[d] += p3;
        }

        /* Update the 2‑fold convolution (c2) */
        for (idx_t d = i; d < current_cap; ++d) {
            if (d >= i)          c2[d] += 2 * an * a_arr[d - i];
            if (d == 2 * i)      c2[d] += p2;
        }

        /* Finally store the newly computed coefficient */
        a_arr[i] = an;
    }

    current_max = n + 1;
}

/* Public sequence accessor */
seq_t a(idx_t n)
{
    if (n >= current_max)
        compute_up_to(n);
    return a_arr[n];
}
