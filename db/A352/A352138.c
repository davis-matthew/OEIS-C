/*
 * A352138  Expansion of e.g.f. 1/(exp(x) - log(1 + x)).
 * https://oeis.org/A352138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = -Sum_{k=1..n} ((-1)^k * (k-1)! + 1) *
 *            binomial(n, k) * a(n-k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute the n-th term of OEIS sequence A352138.
 * The function uses memoisation so that calls in arbitrary order
 * are handled correctly.  Only standard C operators are used on seq_t
 * values, and dynamic memory is allocated with proper casts to avoid
 * C++ conversion warnings. */
seq_t a(idx_t n)
{
    /* ---------- Persistent state ---------- */
    static seq_t *a_vals   = NULL;   /* a[0..max_calc]          */
    static idx_t  max_calc = (idx_t)-1; /* largest n computed      */
    static seq_t *fact_arr = NULL;   /* (k-1)! for k up to max  */
    static idx_t  fact_max = 0;      /* number of factorials stored */

    /* ---------- First call initialisation ---------- */
    if (max_calc == (idx_t)-1) {         /* no values known yet   */
        a_vals   = (seq_t*)malloc(sizeof(seq_t) * 1);
        fact_arr = (seq_t*)malloc(sizeof(seq_t) * 1);
        if (!a_vals || !fact_arr) abort();

        a_vals[0] = (seq_t)1;
        fact_arr[0] = (seq_t)1;          /* 0! = 1                 */

        max_calc = 0;
        fact_max = 1;                    /* stored up to 0! only   */
        return a_vals[0];
    }

    /* ---------- Return cached result if already known ---------- */
    if (n <= max_calc) return a_vals[n];

    /* ---------- Resizing storage for a_vals ---------- */
    if (n >= max_calc) {
        seq_t *tmp = (seq_t*)realloc(a_vals, sizeof(seq_t) * (n + 1));
        if (!tmp) abort();
        a_vals = tmp;
    }

    /* ---------- Ensure factorial table up to n ---------- */
    if (fact_max <= n) {
        seq_t *tmp = (seq_t*)realloc(fact_arr, sizeof(seq_t) * (n + 1));
        if (!tmp) abort();
        fact_arr = tmp;
        for (idx_t i = fact_max; i <= n; ++i) {
            fact_arr[i] = fact_arr[i - 1] * (seq_t)i;   /* (i)!           */
        }
        fact_max = n + 1;
    }

    /* ---------- Compute missing terms ---------- */
    for (idx_t cur = max_calc + 1; cur <= n; ++cur) {
        seq_t sum = (seq_t)0;

        /* binomial(cur,0) = 1, then update multiplicatively */
        seq_t binom = (seq_t)1;
        for (idx_t k = 1; k <= cur; ++k) {
            binom = binom * (seq_t)(cur - k + 1) / (seq_t)k;  /* C(cur,k) */

            /* coefficient: ((-1)^k * (k-1)! + 1) */
            seq_t coeff = fact_arr[k - 1];                      /* (k-1)!   */
            if (k & 1) coeff = -coeff;                         /* (-1)^k   */
            coeff = coeff + (seq_t)1;                          /* +1       */

            /* accumulate          coeff * binom * a(cur-k)               */
            seq_t prod = coeff * binom * a_vals[cur - k];
            sum = sum + prod;
        }
        a_vals[cur] = -sum;   /* final negative sign from formula */
    }

    max_calc = n;
    return a_vals[n];
}
