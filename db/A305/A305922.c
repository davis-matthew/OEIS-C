/*
 * A305922  Expansion of e.g.f. log(1 + 2*x/(exp(x) + 1)).
 * https://oeis.org/A305922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   enlarge
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A305922 : a(n) = n! * [x^n] log(1 + 2*x/(exp(x)+1))
 *  Recurrence: let g_n = n! * [x^n] (1 + 2*x/(exp(x)+1))
 *   – g_0 = 1
 *   – for n ≥ 1   g_n = ( r_n – Σ_{k=0}^{n-1} C(n,k) g_k ) / 2
 *        r_0 = 2, r_1 = 3, r_n = 1  (n ≥ 2)
 *   – where C(n,k) = n!/(k!(n-k)!)
 *
 *  Let a_n = n! * [x^n] log(1 + 2*x/(exp(x)+1))
 *   – a_0 = 0
 *   – a_1 = g_1
 *   – for n ≥ 2
 *        a_n = g_n – Σ_{k=1}^{n-1} C(n-1,k-1) * a_k * g_{n-k}
 *
 *  The function computes g and a on demand, storing the values in
 *  static arrays so repeated calls are fast.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/enlarge.h"

/*  The types seq_t and idx_t are supplied by the build system.
 *  Do not define them here. */

static seq_t *g_arr = NULL;
static seq_t *a_arr = NULL;
static idx_t g_cap = 0;       /* allocated size of g_arr   */
static idx_t a_cap = 0;       /* allocated size of a_arr   */
static idx_t g_used = 0;      /* number of elements in g_arr   */
static idx_t a_used = 0;      /* number of elements in a_arr   */

/*  Compute g[n] and a[n] up to index n, inclusive.  */
static void compute_up_to(idx_t n)
{
    /*  Compute g coefficients first.  */
    enlarge(&g_arr, &g_cap, n + 1);
    if (g_used == 0) {          /* initialisation */
        g_arr[0] = (seq_t)1;    /* g_0 = 1 */
        g_used = 1;
    }
    for (idx_t k = g_used; k <= n; ++k) {
        /* Recurrence for g_k */
        seq_t r = (k == 0) ? (seq_t)2
                 : (k == 1) ? (seq_t)3
                 : (seq_t)1;
        seq_t sum = 0;
        seq_t bin = (seq_t)1;          /* C(k,0) */
        for (idx_t j = 0; j < k; ++j) {
            sum += bin * g_arr[j];
            /* update bin to C(k,j+1) */
            bin = bin * (seq_t)(k - j);
            bin = bin / (seq_t)(j + 1);
        }
        g_arr[k] = (r - sum) / (seq_t)2;
    }
    g_used = n + 1;

    /*  Now compute a coefficients.  */
    enlarge(&a_arr, &a_cap, n + 1);
    if (a_used == 0) {
        a_arr[0] = (seq_t)0;    /* a_0 = 0 */
        a_used = 1;
    }
    for (idx_t k = a_used; k <= n; ++k) {
        if (k == 1) {
            a_arr[1] = g_arr[1];
        } else {
            seq_t sum = 0;
            seq_t bin = (seq_t)1;          /* C(k-1,0) */
            for (idx_t j = 1; j < k; ++j) {
                /* term = C(k-1, j-1) * a_j * g_{k-j} */
                seq_t term = bin * a_arr[j];
                term = term * g_arr[k - j];
                sum += term;
                /* update bin to C(k-1, j) */
                bin = bin * (seq_t)((k - 1) - (j - 1));
                bin = bin / (seq_t)j;
            }
            a_arr[k] = g_arr[k] - sum;
        }
    }
    a_used = n + 1;
}

/*  Public function returning the nth term.  */
seq_t a(idx_t n)
{
    if (n >= a_used)           /* need to extend the cache */
        compute_up_to(n);
    return a_arr[n];
}
