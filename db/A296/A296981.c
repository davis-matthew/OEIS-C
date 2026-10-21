/*
 * A296981  Expansion of e.g.f. arctan(log(1 + x)).
 * https://oeis.org/A296981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A296981
   Formula: a(n) = Σ_{m=0}^{⌊(n‑1)/2⌋} (-1)^m · (2m)! · s1(n, 2m+1)
   where s1(n,k) are the signed Stirling numbers of the first kind
   satisfying s1(0,0)=1, s1(n,0)=0 (n>0), and
   s1(n,k) = s1(n‑1,k‑1) – (n‑1)·s1(n‑1,k).  */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static tables with incremental computation */
    static seq_t *fact   = NULL;   /* factorials: fact[i] = i!  */
    static size_t fact_cap = 0;

    static seq_t *s_prev = NULL;   /* signed Stirling numbers row n‑1  */
    static seq_t *s_curr = NULL;   /* signed Stirling numbers row n    */
    static size_t stir_cap = 0;

    static seq_t *a_val  = NULL;   /* a(n) values                       */
    static size_t a_cap  = 0;

    static idx_t max_n = 0;        /* highest n already computed */

    /*--- initialization for the first call ---------------------------------*/
    if (!fact) {
        /* factorial[0] = 1 */
        fact_cap = 1;
        fact = (seq_t *)realloc(NULL, fact_cap * sizeof(seq_t));
        fact[0] = (seq_t)1;

        /* Stirling tables, capacity 1 */
        stir_cap = 1;
        s_prev = (seq_t *)realloc(NULL, stir_cap * sizeof(seq_t));
        s_curr = (seq_t *)realloc(NULL, stir_cap * sizeof(seq_t));
        s_prev[0] = (seq_t)1;          /* s1(0,0)=1 */
        s_curr[0] = (seq_t)0;          /* dummy */

        /* a(0)=0 */
        a_cap = 1;
        a_val = (seq_t *)realloc(NULL, a_cap * sizeof(seq_t));
        a_val[0] = (seq_t)0;

        max_n = 0;
    }

    /*--- grow factorial table if needed -----------------------------------*/
    if (fact_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        for (size_t i = fact_cap; i < new_cap; ++i) {
            new_fact[i] = new_fact[i - 1] * (seq_t)i;
        }
        fact = new_fact;
        fact_cap = new_cap;
    }

    /*--- grow Stirling tables if needed -----------------------------------*/
    size_t need_cap = (size_t)n + 1;
    if (stir_cap < need_cap) {
        seq_t *new_prev = (seq_t *)realloc(s_prev, need_cap * sizeof(seq_t));
        seq_t *new_curr = (seq_t *)realloc(s_curr, need_cap * sizeof(seq_t));
        for (size_t i = stir_cap; i < need_cap; ++i) {
            new_prev[i] = (seq_t)0;
            new_curr[i] = (seq_t)0;
        }
        s_prev = new_prev;
        s_curr = new_curr;
        stir_cap = need_cap;
    }

    /*--- grow a(n) table if needed ---------------------------------------*/
    if (a_cap < need_cap) {
        seq_t *new_a = (seq_t *)realloc(a_val, need_cap * sizeof(seq_t));
        for (size_t i = a_cap; i < need_cap; ++i) {
            new_a[i] = (seq_t)0;
        }
        a_val = new_a;
        a_cap = need_cap;
    }

    /*--- compute new rows up to n ----------------------------------------*/
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* s_curr[0] = 0 for all n > 0 */
        s_curr[0] = (seq_t)0;

        /* compute Stirling row i */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t val1 = s_prev[k - 1];
            seq_t val2 = (k <= i - 1) ? s_prev[k] : (seq_t)0;
            s_curr[k] = val1 - (seq_t)(i - 1) * val2;
        }

        /* accumulate a(i) */
        seq_t sum = (seq_t)0;
        for (idx_t m = 0; (2 * m + 1) <= i; ++m) {
            seq_t fact_term = fact[2 * m];          /* (2m)!   */
            seq_t s_val = s_curr[2 * m + 1];        /* s1(i, 2m+1) */
            seq_t term = fact_term * s_val;
            if (m & 1) term = -term;               /* (-1)^m */
            sum += term;
        }
        a_val[i] = sum;

        /* swap rows for next iteration */
        seq_t *tmp = s_prev;
        s_prev = s_curr;
        s_curr = tmp;
    }

    max_n = n;
    return a_val[n];
}
