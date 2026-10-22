/*
 * A320654  Expansion of 1/(2 - Product_{k>=1} (1 + x^k)/(1 - x^k)).
 * https://oeis.org/A320654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A320654: a(0)=1; for n≥1,
   a(n) = Σ_{k=1..n} A015128(k) * a(n-k)                            */
/* A015128(k) are the coefficients of the power series expansion of
   1/θ₄(q) where θ₄(q)=∑_{m∈ℤ}(-1)^m q^{m²}.  They satisfy
   r₀=1 and for n≥1: r_n = Σ_{m odd≤√n} 2·r_{n-m²} – Σ_{m even≤√n} 2·r_{n-m²}.  */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *r_arr = NULL;     /* r[0..]   coefficients of 1/θ₄  */
static seq_t *a_arr = NULL;     /* a[0..]   target sequence        */
static idx_t  max_n  = 0;       /* largest n already computed      */
static bool   init   = false;   /* initialization flag             */

seq_t a(idx_t n)
{
    if (!init) {                /* first call: allocate size 1 */
        r_arr = (seq_t *)malloc(sizeof(seq_t));
        a_arr = (seq_t *)malloc(sizeof(seq_t));
        if (!r_arr || !a_arr) abort();   /* insufficient memory */
        r_arr[0] = (seq_t)1;  /* r_0 = 1 */
        a_arr[0] = (seq_t)1;  /* a_0 = 1 */
        init   = true;
        max_n  = 0;
    }

    if (n > max_n) {
        size_t new_cap = (size_t)(n + 1);
        r_arr = (seq_t *)realloc(r_arr, new_cap * sizeof(seq_t));
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        if (!r_arr || !a_arr) abort();

        /*--------------------------------------------------------------------
          Compute the new r_k values, k = max_n+1 .. n
        --------------------------------------------------------------------*/
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t sum_minus = 0, sum_plus = 0;          /* accumulators */
            for (idx_t m = 1; (m * m) <= i; ++m) {
                idx_t j = i - (m * m);
                seq_t term = r_arr[j];
                term += term;   /* multiply by 2: term = 2 * r_j */

                if (m & 1)        /* m odd: contributes to sum_minus   */
                    sum_minus += term;
                else              /* m even: contributes to sum_plus   */
                    sum_plus  += term;
            }
            /* r_i = sum_minus - sum_plus   (sum_minus ≥ sum_plus) */
            r_arr[i] = sum_minus - sum_plus;
        }

        /*--------------------------------------------------------------------
          Compute the new a_k values, k = max_n+1 .. n
        --------------------------------------------------------------------*/
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t acc = 0;
            for (idx_t k = 1; k <= i; ++k)
                acc += r_arr[k] * a_arr[i - k];
            a_arr[i] = acc;
        }

        max_n = n;
    }
    return a_arr[n];
}
