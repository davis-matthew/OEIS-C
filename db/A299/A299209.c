/*
 * A299209  Expansion of 1/(1 - x*Product_{k>=1} (1 - k*x^k)).
 * https://oeis.org/A299209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Formula (OEIS A299209): 
   a(0) = 1; 
   a(n) = Σ_{k=1..n} A022661(k‑1) * a(n‑k)   for n≥1
   where A022661(k‑1) are the coefficients of 
   ∏_{k≥1} (1 - k·x^k). 
   The coefficients are computed by multiplying the factors
   (1 - k·x^k) one by one.
*/

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    idx_t N = n;
    /* allocate space for the coefficients of the product up to degree N‑1
       and for the sequence values up to N */
    seq_t *h = (seq_t *)malloc((size_t)N * sizeof(seq_t));
    seq_t *a_arr = (seq_t *)malloc((size_t)(N + 1) * sizeof(seq_t));
    if (!h || !a_arr) {
        /* In case of allocation failure, return 0 as a safe value. */
        if (h) free(h);
        if (a_arr) free(a_arr);
        return (seq_t)0;
    }

    /* initialise */
    h[0] = (seq_t)1;
    for (idx_t i = 1; i < N; ++i) {
        h[i] = (seq_t)0;
    }
    a_arr[0] = (seq_t)1;

    /* compute the coefficients of ∏_{k≥1} (1 - k·x^k) up to degree N-1 */
    for (idx_t k = 1; k < N; ++k) {
        for (idx_t j = N - 1; j >= k; --j) {
            h[j] -= (seq_t)k * h[j - k];
        }
    }

    /* compute the sequence values using the recurrence */
    for (idx_t i = 1; i <= N; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += h[k - 1] * a_arr[i - k];
        }
        a_arr[i] = sum;
    }

    seq_t result = a_arr[N];
    free(h);
    free(a_arr);
    return result;
}
