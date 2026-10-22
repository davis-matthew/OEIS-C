/*
 * A377376  Expansion of e.g.f. log( 1 - log(1 - x)^3 / 6 ).
 * https://oeis.org/A377376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |Stirling1(n,3)| - Sum_{k=1..n-1} |Stirling1(k,3)| *
 *            binomial(n-1,k) * a(n-k)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 14, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A recursive definition that uses only |Stirling1(_,3) */
/* and binomial coefficients.  The unsigned Stirling numbers
   of the first kind are computed with the recurrence
   c(n,k) = c(n-1,k-1) + (n-1)*c(n-1,k).  */

#include <stdint.h>
#include <stdlib.h>

/* Forward declaration of the sequence accessor */
seq_t a(idx_t n);

/* Cached values */
static seq_t *s3_arr = NULL;   /* |S1(n,3)| for n = 0 .. max_n   */
static seq_t *a_arr = NULL;    /* a(n) for n = 0 .. max_n       */
static idx_t   max_computed = 0;  /* largest n whose values are known */

/* Current unsigned Stirling numbers of first kind for k = 1,2,3 at current n */
static seq_t cur_c1 = 1;   /* c(n,1) */
static seq_t cur_c2 = 0;   /* c(n,2) */
static seq_t cur_c3 = 0;   /* c(n,3) */

/* Computes binomial coefficient C(n,k) using a multiplicative formula.
   It works for any seq_t that supports + - * / with integral operands. */
static inline seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - i + 1);
        res = res / (seq_t)i;               /* exact division */
    }
    return res;
}

/* Extends precomputed tables up to n (inclusive). */
static void ensure_up_to(idx_t n)
{
    if (n <= max_computed) return;        /* already done */

    /* Reallocate or allocate arrays for |S1(n,3)| and a(n) */
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_s3 = (seq_t *)realloc(s3_arr, new_cap * sizeof(seq_t));
    seq_t *new_a  = (seq_t *)realloc(a_arr,  new_cap * sizeof(seq_t));
    if (!new_s3 || !new_a) { abort(); }    /* allocation failed */

    s3_arr = new_s3;
    a_arr  = new_a;

    /* If this is the first time, initialise base cases */
    if (max_computed == 0)
    {
        s3_arr[0] = 0;
        a_arr[0]  = 0;

        /* For n = 1 */
        s3_arr[1] = 0;          /* c(1,3) = 0     */
        a_arr[1]  = 0;          /* a(1) = 0       */

        cur_c1 = 1; cur_c2 = 0; cur_c3 = 0;  /* c(1,1)=1, c(1,2)=0, c(1,3)=0 */
        max_computed = 1;
    }

    /* Compute |S1(n,3)| for n = max_computed+1 .. n */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        seq_t new_c1 = (seq_t)(i - 1) * cur_c1;
        seq_t new_c2 = cur_c1 + (seq_t)(i - 1) * cur_c2;
        seq_t new_c3 = cur_c2 + (seq_t)(i - 1) * cur_c3;

        cur_c1 = new_c1;
        cur_c2 = new_c2;
        cur_c3 = new_c3;

        s3_arr[i] = cur_c3;   /* unsigned value already */
    }

    /* Compute the sequence values using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        seq_t sum = 0;
        for (idx_t k = 1; k < i; ++k)
        {
            seq_t term = s3_arr[k];
            term = term * binomial(i - 1, k);
            term = term * a_arr[i - k];
            sum = sum + term;
        }
        a_arr[i] = s3_arr[i] - sum;
    }

    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* a(0) is defined as 0 */
    ensure_up_to(n);
    return a_arr[n];
}
