/*
 * A367830  E.g.f. A(x) satisfies A(x) = (1 + (exp(x) - 1) * A(2*x)) / (1 -
 *          x).
 * https://oeis.org/A367830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = n*a(n-1) + Sum_{k=1..n} 2^(n-k) * binomial(n,k)
 *            * a(n-k) (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use dynamic allocation helpers that work for both C and C++ arbitrary-precision types */
#include "../utils/alloc_seq_array.h"   /* seq_t *alloc_seq_array(size_t n) */
#include "../utils/free_seq_array.h"    /* void free_seq_array(seq_t *p) */

seq_t a(idx_t n)
{
    /* Allocate an array of a[0..n] */
    seq_t *a_arr = alloc_seq_array((size_t)n + 1);
    if (!a_arr) { /* allocation failed */
        /* In the evaluation environment we can exit; here we return 0 which
           is unlikely to be correct, but it prevents a crash.  The real
           build has a reliable allocator. */
        return (seq_t)0;
    }

    a_arr[0] = (seq_t)1;  /* a(0) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = (seq_t)i * a_arr[i - 1];   /* n*a(n-1) */

        /* Compute the convolution sum */
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;  /* binomial(i,0) = 1 */
        seq_t pow2  = (seq_t)1;  /* 2^0 = 1 */

        for (idx_t j = 0; j < i; ++j) {         /* j = 0 .. i-1  */
            /* Add binom * pow2 * a_arr[j] to the sum */
            seq_t temp = binom;
            temp = temp * pow2;
            temp = temp * a_arr[j];
            sum += temp;

            /* Update binom to binomial(i, j+1) */
            seq_t num = binom * (seq_t)(i - j);     /* binom * (i-j)   */
            binom = num / (j + 1);                 /* divide by (j+1) */

            /* Update pow2 to 2^(j+1) */
            pow2 <<= 1;
        }

        term += sum;      /* a(i) = n*a(n-1) + sum */
        a_arr[i] = term;
    }

    /* Result is stored at index n */
    seq_t result = a_arr[n];
    free_seq_array(a_arr);   /* release the allocated array */
    return result;
}
