/*
 * A328776  Product_{n>=1} (1 + x^n)^a(n) = 1 + Sum_{n>=1} sigma(n) * x^n,
 *          where sigma = A000203.
 * https://oeis.org/A328776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) computed by inverting the weighted transform of σ(n) with
 *            the product ∏_{n≥1} (1 + x^n)^a(n) = 1 + Σ_{n≥1} σ(n) x^n. The
 *            calculation uses the following recurrence: Mc[1] = σ(1) Mc[n] =
 *            n·σ(n) – Σ_{k=1}^{n-1} σ(n–k)·Mc[k] (1) A[n] = Mc[n] – Σ_{d|n,
 *            d<n} a(d)·(-1)^{n/d+1}·d (2) a(n) = A[n] / n...
 * Range:     i8 n <= 15, i16 n <= 39, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t max_computed = 0;
static seq_t *sigma_arr   = NULL;   /* σ(n) */
static seq_t *Mc_arr      = NULL;   /* Mc[n] = n * c_n   */
static seq_t *a_arr       = NULL;   /* result sequence a(n)   */

/* Compute sigma array up to n */
static void compute_sigma(idx_t n)
{
    /* allocate new array */
    sigma_arr = (seq_t*)realloc(sigma_arr, (size_t)(n + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= n; ++i) sigma_arr[i] = 0;

    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            sigma_arr[j] += (seq_t)i;
        }
    }
}

/* Main function: returns a(n) */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* defined only for n>=1 */

    /* If we haven't computed up to n yet, compute all arrays */
    if (n > max_computed) {
        compute_sigma(n);

        /* allocate arrays up to n */
        Mc_arr = (seq_t*)realloc(Mc_arr, (size_t)(n + 1) * sizeof(seq_t));
        a_arr  = (seq_t*)realloc(a_arr,  (size_t)(n + 1) * sizeof(seq_t));

        /* compute Mc[1..n] using recurrence (1) */
        Mc_arr[0] = 0;  /* unused */
        Mc_arr[1] = sigma_arr[1];
        for (idx_t i = 2; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k < i; ++k) {
                seq_t term = sigma_arr[i - k] * Mc_arr[k];
                sum += term;
            }
            Mc_arr[i] = (seq_t)i * sigma_arr[i] - sum;
        }

        /* compute a[1..n] using recurrence (2) */
        a_arr[0] = 0;  /* unused */
        for (idx_t i = 1; i <= n; ++i) {
            seq_t sumdiv = 0;
            /* iterate over proper divisors of i */
            for (idx_t d = 1; d * d <= i; ++d) {
                if (i % d == 0) {
                    idx_t d1 = d;
                    idx_t d2 = i / d;
                    if (d1 < i) {
                        seq_t sign = (( (i / d1) + 1) & 1) ? (seq_t)-1 : (seq_t)1;
                        sumdiv += a_arr[d1] * ((seq_t)d1) * sign;
                    }
                    if (d2 != d1 && d2 < i) {
                        seq_t sign = (( (i / d2) + 1) & 1) ? (seq_t)-1 : (seq_t)1;
                        sumdiv += a_arr[d2] * ((seq_t)d2) * sign;
                    }
                }
            }
            seq_t A_i = Mc_arr[i] - sumdiv;
            a_arr[i] = A_i / (seq_t)i;  /* division is exact */
        }

        max_computed = n;
    }

    return a_arr[n];
}
