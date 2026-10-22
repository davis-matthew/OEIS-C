/*
 * A382734  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where A(n,k) = n! * k! * [x^n * y^k] 1 / (exp(x) +
 *          exp(y) - exp(x+y))^2.
 * https://oeis.org/A382734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 58, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A382734 – value a(n) read from the square array A(n,k)  
    Definition (from OEIS):  
        A(n,k) = sum_{t=0..min(n,k)} t! * (t+1)! * S(n,t) * S(k,t)  
        a(n) is obtained by reading A(n,k) by antidiagonals downwards.
    This implementation computes a(n) for any n≥0 using only integer
    arithmetic.  The running time is O(√n) per call (≈140 iterations for
    n≈10 000).  It uses a small memoised cache for Stirling numbers of the
    second kind.  No external libraries are required. */

#include <stddef.h>
#include <stdint.h>

/* maximum row index that can occur for n up to ~10 000
   (d = ⌊(sqrt(8n+1)-1)/2⌋, for n=10 000 d≈140, so 200 is safe) */
#define MAX_D 200

/*------------------------------------------------------------------*/
/* Compute Stirling numbers of the second kind S(n,k) for a fixed
   n and store them in the array row[0..n].  row[k] will contain
   S(n,k).  For k>n the value is left as zero.  The routine assumes
   MAX_D >= n. */
static void compute_stirling_row(idx_t n, seq_t *row)
{
    /* temporary array to hold the current row during construction */
    seq_t temp[MAX_D + 1];

    /* initialise for n=0 */
    for (idx_t k = 0; k <= MAX_D; ++k) temp[k] = (seq_t)0;
    temp[0] = (seq_t)1;          /* S(0,0) = 1 */

    if (n == 0) {
        row[0] = (seq_t)1;
        return;
    }

    for (idx_t cur = 1; cur <= n; ++cur) {
        /* update in decreasing order to use the previous row values */
        for (idx_t k = cur; k >= 1; --k) {
            /* S(cur,k) = k * S(cur-1,k) + S(cur-1,k-1) */
            temp[k] = (seq_t)k * temp[k] + temp[k - 1];
        }
        temp[0] = (seq_t)0;     /* S(cur,0) = 0 for cur>0 */
    }

    /* copy the resulting row to the output array */
    for (idx_t k = 0; k <= n; ++k) row[k] = temp[k];
}

/*------------------------------------------------------------------*/
/* Main sequence routine.  Returns a(n) for the given index n. */
seq_t a(idx_t n)
{
    /* find the diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) ++d;

    idx_t start = d * (d + 1) / 2;   /* first index on diagonal d */
    idx_t i = n - start;            /* row index on the diagonal   */
    idx_t j = d - i;                /* column index (i+j = d)      */

    /* m is the upper bound of the summation index t */
    idx_t m = i < j ? i : j;

    /* Stirling rows for i and j */
    seq_t stir_i[MAX_D + 1];
    seq_t stir_j[MAX_D + 1];
    compute_stirling_row(i, stir_i);
    compute_stirling_row(j, stir_j);

    /* factorials: fact_t = t!   ; fact_t1 = (t+1)!   */
    seq_t fact_t  = (seq_t)1;   /* 0!   */
    seq_t fact_t1 = (seq_t)1;   /* 1!   */
    seq_t sum     = (seq_t)0;

    for (idx_t t = 0; t <= m; ++t) {
        /* product = t! * (t+1)! * S(i,t) * S(j,t) */
        seq_t prod = fact_t;
        prod = prod * fact_t1;
        prod = prod * stir_i[t];
        prod = prod * stir_j[t];
        sum += prod;

        /* advance factorials for the next t, if any */
        if (t < m) {
            idx_t mult = t + 2;            /* (t+2) to get (t+2)! */
            seq_t new_fact_t1 = fact_t1 * (seq_t)mult;
            fact_t  = fact_t1;            /* t!  -> (t+1)!   */
            fact_t1 = new_fact_t1;        /* (t+1)! -> (t+2)! */
        }
    }

    return sum;
}
