/*
 * A259829  a(n) = (-1)^floor(n/2) * A035185(n).
 * https://oeis.org/A259829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^floor(n/2) * A035185(n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation based on the generating function:
   a(n) is the coefficient of q^n in
   Sum_{k>=1} -(-1)^k q^{k(k+1)/2} (1-q)(1-q^2)…(1-q^{k-1}) /
   (1+q)(1+q^2)…(1+q^k)                               */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* static storage for memoised values */
static seq_t *ans_arr = NULL;        /* a(0)..a(max)          */
static seq_t *R_arr   = NULL;        /* R polynomial up to max */
static seq_t *E_arr   = NULL;        /* temporary E array      */
static seq_t *T_arr   = NULL;        /* temporary T array      */
static size_t  max_alloc = 0;        /* current allocation size   */
static idx_t   max_computed = 0;     /* largest n for which a() has been computed */

/* Ensure that all arrays are allocated for indices up to n (inclusive) */
static void ensure_arrays(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed > max_alloc) {
        free(ans_arr); free(R_arr); free(E_arr); free(T_arr);
        ans_arr = (seq_t*)calloc(needed, sizeof(seq_t));
        R_arr   = (seq_t*)calloc(needed, sizeof(seq_t));
        E_arr   = (seq_t*)calloc(needed, sizeof(seq_t));
        T_arr   = (seq_t*)calloc(needed, sizeof(seq_t));
        max_alloc = needed;
    }
}

/* Compute a(n) for all indices up to n, memoising the results */
static void compute_up_to(idx_t n)
{
    if (n <= max_computed) return;        /* already available */

    ensure_arrays(n);

    /* reset arrays for fresh computation */
    memset(ans_arr, 0, (n + 1) * sizeof(seq_t));
    memset(R_arr,   0, (n + 1) * sizeof(seq_t));
    R_arr[0] = 1;                          /* R_0 = 1 */

    idx_t k;
    for (k = 1; ; ++k) {
        idx_t shift = (k * (k + 1)) / 2;
        if (shift > n) break;              /* no further contributions */

        idx_t maxM = n - shift;

        /* Compute E = R_{k-1} / (1 + q^k) truncated to degree maxM */
        E_arr[0] = R_arr[0];
        for (idx_t m = 1; m <= maxM; ++m) {
            if (m >= k)
                E_arr[m] = R_arr[m] - E_arr[m - k];
            else
                E_arr[m] = R_arr[m];
        }

        /* Add contributions to a[] */
        seq_t sign = (((k + 1) & 1) ? -1 : 1);   /* (-1)^{k+1} */
        for (idx_t m = 0; m <= maxM; ++m) {
            ans_arr[shift + m] += sign * E_arr[m];
        }

        /* Update R: R_k = R_{k-1} * (1 - q^k) / (1 + q^k) */
        /* Step 1: multiply by (1 - q^k) into T_arr */
        T_arr[0] = R_arr[0];
        for (idx_t i = 1; i <= n; ++i) {
            if (i >= k)
                T_arr[i] = R_arr[i] - R_arr[i - k];
            else
                T_arr[i] = R_arr[i];
        }
        /* Step 2: divide T_arr by (1 + q^k) back into R_arr */
        R_arr[0] = T_arr[0];
        for (idx_t i = 1; i <= n; ++i) {
            if (i >= k)
                R_arr[i] = T_arr[i] - R_arr[i - k];
            else
                R_arr[i] = T_arr[i];
        }
    }

    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* not used, but safety */
    compute_up_to(n);
    return ans_arr[n];
}
