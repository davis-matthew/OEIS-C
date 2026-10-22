/*
 * A355097  E.g.f. A(x) satisfies A(x) = 1 + 3 * log(1+x) * A(log(1+x)).
 * https://oeis.org/A355097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3 * Σ_{k=1..n} k * S1(n,k) * a(k-1) where S1(n,k) are the
 *            signed Stirling numbers of the first kind. a(0) = 1. Caching is
 *            used so that calls are fast in any order
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* ---------- static cache for values of a(0..max_computed) ---------- */
    static idx_t   max_computed = 0;   /* highest index that has been computed */
    static seq_t * a_vals      = NULL; /* array of a values              */
    static size_t a_cap       = 0;     /* allocated capacity             */

    /* ---------- static buffers for one row of Stirling numbers ---------------- */
    static seq_t * s_prev      = NULL; /* previous row S1(m-1,·)          */
    static seq_t * s_curr      = NULL; /* current row S1(m,·)             */
    static size_t s_cap       = 0;     /* allocated capacity              */

    /* Ensure the arrays are large enough for indices up to n */
    if (n + 1 > a_cap) {
        /* compute new capacity */
        size_t new_cap = (size_t)(n + 1);

        /* grow or allocate a_vals */
        if (a_vals == NULL)
            a_vals = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            a_vals = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
        a_cap = new_cap;

        /* grow or allocate Stirling buffers */
        if (s_prev == NULL)
            s_prev = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            s_prev = (seq_t *)realloc(s_prev, new_cap * sizeof(seq_t));

        if (s_curr == NULL)
            s_curr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            s_curr = (seq_t *)realloc(s_curr, new_cap * sizeof(seq_t));
        s_cap = new_cap;

        /* initialize base cases if this is the very first call */
        if (max_computed == 0) {
            a_vals[0] = (seq_t)1;             /* a(0) = 1               */
            s_prev[0] = (seq_t)1;             /* S1(0,0) = 1            */
            for (size_t i = 1; i < new_cap; ++i)
                s_prev[i] = (seq_t)0;         /* S1(0,k>0) = 0          */
        }
    }

    /* ----------------------------------------------------------- */
    /* compute values up to n (if necessary)                      */
    /* ----------------------------------------------------------- */
    for (idx_t m = max_computed + 1; m <= n; ++m) {
        /* ----- compute the Stirling row for n = m ----- */
        s_curr[0] = (seq_t)0;                 /* S1(m,0) = 0             */
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term1 = s_prev[k - 1];
            seq_t term2 = (seq_t)(m - 1) * s_prev[k];
            s_curr[k] = term1 - term2;        /* recurrence              */
        }

        /* ----- compute a(m) from the new Stirling numbers ----- */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t prod = s_curr[k] * a_vals[k - 1]; /* a(k-1) already known */
            prod *= (seq_t)k;                       /* multiply by k       */
            sum += prod;                            /* accumulate          */
        }
        a_vals[m] = (seq_t)3 * sum;                /* final a(m)          */

        /* swap the Stirling buffers to prepare for next iteration */
        seq_t *tmp = s_prev;
        s_prev = s_curr;
        s_curr = tmp;
    }

    max_computed = n;
    return a_vals[n];
}
