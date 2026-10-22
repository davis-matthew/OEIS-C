/*
 * A367845  Expansion of e.g.f. 1/(1 - x + log(1 - 2*x)).
 * https://oeis.org/A367845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + Sum_{k=1..n} 2^k * (k-1)! * binomial(n,k) *
 *            a(n-k), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Computes the n-th term of OEIS A367845.
   The function is safe for arbitrary-precision seq_t as well as
   for 8, 16, 32, 64 or 128 bit unsigned integers.   */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* allocate three arrays of size (n+1) */
    size_t len = (size_t)(n + 1);
    seq_t *a_arr  = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *pow2   = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *fact   = (seq_t *)malloc(len * sizeof(seq_t));

    if (!a_arr || !pow2 || !fact) {
        /* In a real library you might handle the error differently */
        exit(1);
    }

    /* initial values */
    a_arr[0] = (seq_t)1;
    pow2[0]  = (seq_t)1;
    fact[0]  = (seq_t)1;

    /* precompute powers of two and factorials */
    for (idx_t i = 1; i <= n; ++i) {
        pow2[i] = pow2[i - 1] * (seq_t)2;
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    /* main recurrence */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;   /* C(i,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* compute binomial coefficient C(i,k) iteratively */
            binom = binom * ( (seq_t)(i - k + 1) ) / (seq_t)k;

            /* term = 2^k * (k-1)! * C(i,k) * a(i-k) */
            seq_t term = pow2[k];
            term = term * fact[k - 1];
            term = term * binom;
            term = term * a_arr[i - k];

            sum += term;
        }

        a_arr[i] = (seq_t)i * a_arr[i - 1] + sum;
    }

    seq_t result = a_arr[n];
    free(a_arr);
    free(pow2);
    free(fact);
    return result;
}
