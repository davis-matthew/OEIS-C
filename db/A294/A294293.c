/*
 * A294293  E.g.f.: exp(1/((1+x)*(1+x^2)*(1+x^3)*(1+x^4)*(1+x^5)) - 1).
 * https://oeis.org/A294293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   e.g.f. exp(1/((1+x)*(1+x^2)*(1+x^3)*(1+x^4)*(1+x^5)) - 1)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/*  coefficients of (1+x)(1+x^2)(1+x^3)(1+x^4)(1+x^5) */
static const seq_t ccoeffs[16] = {
    /* 0 */ (seq_t)1,  /* empty product */
    /* 1 */ (seq_t)1,  /* 1          */
    /* 2 */ (seq_t)1,  /* 2          */
    /* 3 */ (seq_t)2,  /* 3,1+2      */
    /* 4 */ (seq_t)2,  /* 4,1+3      */
    /* 5 */ (seq_t)3,  /* 5,1+4,2+3  */
    /* 6 */ (seq_t)3,  /* 1+5,2+4,1+2+3 */
    /* 7 */ (seq_t)3,  /* 2+5,3+4,1+2+4 */
    /* 8 */ (seq_t)3,  /* 3+5,1+2+5,1+3+4 */
    /* 9 */ (seq_t)3,  /* 4+5,1+3+5,2+3+4 */
    /*10 */ (seq_t)3,  /* 1+4+5,2+3+5,1+2+3+4 */
    /*11 */ (seq_t)2,  /* 2+4+5,1+2+3+5   */
    /*12 */ (seq_t)2,  /* 3+4+5,1+2+4+5   */
    /*13 */ (seq_t)1,  /* 1+3+4+5         */
    /*14 */ (seq_t)1,  /* 2+3+4+5         */
    /*15 */ (seq_t)1   /* 1+2+3+4+5       */
};

static seq_t *d_vals = NULL;      /* coefficients of 1/((1+x)...(1+x^5))  */
static seq_t *a_vals = NULL;      /* result sequence (optional memoization) */
static size_t d_cap = 0, a_cap = 0;
static idx_t  d_max = 0, a_max = 0;

/* Ensure sufficient capacity for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;

    if (d_cap < needed) {
        size_t new_cap = d_cap ? d_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(d_vals, new_cap * sizeof(seq_t));
        d_vals = tmp;
        d_cap = new_cap;
    }
    if (a_cap < needed) {
        size_t new_cap = a_cap ? a_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(a_vals, new_cap * sizeof(seq_t));
        a_vals = tmp;
        a_cap = new_cap;
    }
}

/* Compute d_vals and a_vals up to n. */
static void compute_upto(idx_t n)
{
    ensure_capacity(n);

    /* Initialise base coefficients if this is the first call. */
    if (d_max == 0 && d_vals != NULL)
        d_vals[0] = (seq_t)1;
    if (a_max == 0 && a_vals != NULL)
        a_vals[0] = (seq_t)1;

    /* 1/((1+x)...(1+x^5)) coefficients */
    if (d_max < n) {
        for (idx_t i = d_max + 1; i <= n; ++i) {
            seq_t sum = 0;
            idx_t limit = i < 15 ? i : 15;
            for (idx_t k = 1; k <= limit; ++k) {
                sum += ccoeffs[k] * d_vals[i - k];
            }
            d_vals[i] = -sum;   /* recurrence: d[i] = -∑ s[k] * d[i-k] */
        }
        d_max = n;
    }

    /* Exponential generating function coefficients */
    if (a_max < n) {
        for (idx_t i = a_max + 1; i <= n; ++i) {
            seq_t total   = 0;
            seq_t falling = 1;              /* (i-1)_{k-1} product */
            for (idx_t k = 1; k <= i; ++k) {
                if (k > 1)
                    falling *= (seq_t)(i - k + 1);   /* accumulate to (i-1)_{k-1} */
                seq_t term = d_vals[k] * a_vals[i - k] * falling * (seq_t)k;
                total += term;
            }
            a_vals[i] = total;
        }
        a_max = n;
    }
}

/* Public interface to obtain a(n). */
seq_t a(idx_t n)
{
    compute_upto(n);
    return a_vals[n];
}
