/*
 * A307593  Expansion of e.g.f. (sec(x) + tan(x))*exp(x)/(1 - x).
 * https://oeis.org/A307593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A307593: Expansion of e.g.f. (sec(x)+tan(x))*exp(x)/(1-x)
    Recurrence (derived from the defining generating function):

           E_0 = 1
           E_{m+1} = ( Σ_{k=0}^{m} binom(m,k) * E_k * E_{m-k} + δ_{m,0} ) / 2   (Euler zigzag numbers)

        f_n   = Σ_{k=0}^{n} binom(n,k) * E_k
        a_0   = 1
        a_n   = n * a_{n-1} + f_n    (n ≥ 1)

    All arithmetic is performed on seq_t (an unsigned integer type of any bit width or arbitrary precision).
    The function works for any n ≥ 0, with calls in any order and memoises all results.
*/

#include <stdlib.h>
#include <stdint.h>

static seq_t *E_arr = NULL;      /* Euler zigzag numbers E_k              */
static seq_t *f_arr = NULL;      /* binomial transforms f_k               */
static seq_t *a_arr = NULL;      /* desired sequence a_k                  */
static idx_t  max_comp = (idx_t)-1;   /* highest index computed (wrap around for -1) */

static void
ensure_up_to(idx_t n)
{
    /* Allocate arrays on first call */
    if (E_arr == NULL) {
        size_t sz = sizeof(seq_t);
        E_arr = (seq_t *)malloc(sz);
        f_arr = (seq_t *)malloc(sz);
        a_arr = (seq_t *)malloc(sz);
        if (!E_arr || !f_arr || !a_arr) exit(1);  /* abort on OOM */

        E_arr[0] = (seq_t)1;
        f_arr[0] = (seq_t)1;
        a_arr[0] = (seq_t)1;
        max_comp = 0;
    }

    /* Nothing to do if already computed */
    if (max_comp >= n) return;

    /* Grow arrays to hold indices up to n */
    size_t new_cap = (size_t)(n + 1);
    E_arr = (seq_t *)realloc(E_arr, new_cap * sizeof(seq_t));
    f_arr = (seq_t *)realloc(f_arr, new_cap * sizeof(seq_t));
    a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    if (!E_arr || !f_arr || !a_arr) exit(1);   /* abort on OOM */

    /* Compute new values sequentially */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        /* ---- compute Euler number E_i ---- */
        seq_t conv_sum = (seq_t)0;
        seq_t binom = (seq_t)1;   /* binom(i-1,0) */
        for (idx_t k = 0; k < i; ++k) {
            seq_t term = E_arr[k] * E_arr[i - 1 - k] * binom;
            conv_sum += term;
            if (k + 1 < i) {
                /* binom := binom(i-1, k+1) = binom * (i-1-k) / (k+1) */
                binom = binom * ((idx_t)(i - 1 - k)) / ((idx_t)(k + 1));
            }
        }
        if (i == 1)          /* add the δ_{i-1,0} = 1 when computing E_1 */
            conv_sum += (seq_t)1;
        E_arr[i] = conv_sum / (seq_t)2;

        /* ---- compute f_i = Σ_{k=0}^{i} binom(i,k) * E_k ---- */
        seq_t f_sum = (seq_t)0;
        binom = (seq_t)1;   /* binom(i,0) */
        for (idx_t k = 0; k <= i; ++k) {
            f_sum += E_arr[k] * binom;
            if (k + 1 <= i) {
                /* binom := binom(i, k+1) = binom * (i-k) / (k+1) */
                binom = binom * ((idx_t)(i - k)) / ((idx_t)(k + 1));
            }
        }
        f_arr[i] = f_sum;

        /* ---- compute a_i = i * a_{i-1} + f_i ---- */
        if (i == 0)
            a_arr[i] = (seq_t)1;
        else
            a_arr[i] = ((seq_t)i) * a_arr[i - 1] + f_sum;
    }

    max_comp = n;
}

/* --------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    ensure_up_to(n);
    return a_arr[n];
}
