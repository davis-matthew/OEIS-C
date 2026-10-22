/*
 * A368209  a(n) = Sum_{k=0..n} BarnesG(k)*BarnesG(n-k).
 * https://oeis.org/A368209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} BarnesG(k)*BarnesG(n-k)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 9, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: BarnesG(0) is defined as 0, BarnesG(1)=1 and for n>=2
   BarnesG(n) = product_{i=1}^{n-2} i!.  */

/* --------------------------------------------------------------
   Computes the OEIS sequence A368209.
   --------------------------------------------------------------
   Requirements:
   1. No external libraries except the standard C library.
   2. Uses only integer arithmetic compatible with seq_t.
   3. Works for any n (small or large) but is efficient for n <= 1000.
   4. All allocations are freed before return.
   -------------------------------------------------------------- */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) = 0, a(1) = 0 */
    if (n <= 1) {
        return (seq_t)0;
    }

    /* Allocate factorial array fact[j] = j!  (0 <= j <= n-2) */
    size_t fact_size = (size_t)(n >= 2 ? n - 1 : 1);
    seq_t *fact = (seq_t *)malloc(fact_size * sizeof(seq_t));
    if (!fact) return (seq_t)0; /* allocation failure, return 0 */

    fact[0] = (seq_t)1;               /* 0! = 1 */
    for (idx_t j = 1; j < fact_size; ++j) {
        fact[j] = fact[j - 1] * ((seq_t)j);   /* j! = (j-1)! * j */
    }

    /* Allocate BarnesG array b[0..n] */
    size_t b_size = (size_t)(n + 1);
    seq_t *b = (seq_t *)malloc(b_size * sizeof(seq_t));
    if (!b) {
        free(fact);
        return (seq_t)0;
    }

    /* Initialise special values */
    b[0] = (seq_t)0;
    b[1] = (seq_t)1;
    if (n >= 2)
        b[2] = (seq_t)1;   /* product of zero terms is 1 */

    /* Compute BarnesG values for n >= 3:
       b[i] = b[i-1] * (i-2)!  */
    for (idx_t i = 3; i <= n; ++i) {
        b[i] = b[i - 1] * fact[i - 2];
    }

    /* Compute the convolution sum a(n) = sum_{k=1}^{n-1} b[k]*b[n-k] */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n - 1; ++k) {
        sum += b[k] * b[n - k];
    }

    /* Clean up */
    free(b);
    free(fact);

    return sum;
}
