/*
 * A370148  Expansion of A(x) = [ Sum_{n>=0} (-7)^(n*(n-1)/2) * (1 +
 *          (-7)^(2*n+1))/(-6) * x^(n*(n+1)/2) ]^(1/3).
 * https://oeis.org/A370148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 0, i16 n <= 2, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   pow_minus7_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A370148
   a(0) = 1
   A(x)^3 = Σ_{n≥0} (-7)^{n(n-1)/2} * (1 + (-7)^{2n+1})/(-6) * x^{n(n+1)/2}
   a(n) = coeff. of x^n in A(x)

   Recurrence derived from
       g_n = Σ_{i+j+k=n} a_i a_j a_k
   (with a_0 = 1).  For n ≥ 1
       a_n = (g_n – Σ_{k=0}^{n-1} (Σ_{i+j=n‑k} a_i a_j) * a_k) / 3
   The inner double sum is maintained via a running convolution 
   array conv2[s] = Σ_{i+j=s} a_i a_j (indices ≤ current maximum).
   The algorithm runs in Θ(n²) time for all terms up to MAX_N, which
   comfortably satisfies the performance requirement for n ≤ 10 000. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/pow_minus7_int.h"

/* Do not change MAX_N – the build defines an appropriate bound. */
#ifndef MAX_N
#define MAX_N 20001
#endif

static seq_t a_arr[MAX_N];              /* a[n] values */
static seq_t g_arr[MAX_N];              /* g[n] = coeff. of x^n in A(x)^3 */
static seq_t conv2[2 * MAX_N + 1];     /* convolution of a with itself */
static idx_t   a_max_computed = 0;     /* highest n for which a[n] is known */
static int initialized = 0;

/* Initialise all tables and pre‑compute g[n] for triangular indices. */
static void init_arrays(void)
{
    if (initialized)
        return;

    /* a[0] = 1 and A(0)^3 = 1. */
    a_arr[0] = 1;
    g_arr[0] = 1;
    conv2[0] = 1;              /* a0 * a0 */

    /* pre‑compute the right‑hand side of A(x)^3 for n = t(t+1)/2 */
    for (idx_t t = 1; ; ++t) {
        idx_t tri = t * (t + 1) / 2;
        if (tri >= MAX_N)
            break;
        /* (-7)^{t(t-1)/2} * (1 + (-7)^{2t+1}) / (-6) */
        seq_t p1 = pow_minus7_int(t * (t - 1) / 2);
        seq_t p2 = pow_minus7_int(2 * t + 1);
        seq_t num = p1 * (seq_t)(1 + p2);
        g_arr[tri] = num / (seq_t)(-6);    /* exact division */
    }
    a_max_computed = 0;
    initialized = 1;
}

/* Compute all a[n] values up to nmax (inclusive). */
static void compute_a_upto(idx_t nmax)
{
    init_arrays();
    if (nmax <= a_max_computed)
        return;

    for (idx_t m = a_max_computed + 1; m <= nmax; ++m) {
        /* Compute T = Σ_{k=0}^{m-1} conv2[m-k] * a[k] */
        seq_t T = 0;
        for (idx_t k = 0; k < m; ++k) {
            idx_t s = m - k;           /* s ∈ [1, m] */
            T += conv2[s] * a_arr[k];
        }

        /* a[m] = (g[m] – T) / 3   (exact division) */
        a_arr[m] = (g_arr[m] - T) / (seq_t)3;

        /* Update conv2 to include pairs involving a[m] */
        for (idx_t i = 0; i <= m; ++i) {
            if (i < m) {
                /* pairs (i,m) and (m,i) → contribute twice */
                conv2[m + i] += (seq_t)2 * a_arr[m] * a_arr[i];
            } else { /* i == m, pair (m,m) counted once */
                conv2[m + i] += a_arr[m] * a_arr[m];
            }
        }
    }
    a_max_computed = nmax;
}

seq_t a(idx_t n)
{
    if (n >= MAX_N)            /* out of pre‑computed range */
        return (seq_t)0;
    compute_a_upto(n);
    return a_arr[n];
}
