/*
 * A356752  E.g.f. satisfies A(x) = 1/(1 - x)^(x^2/2 * A(x)).
 * https://oeis.org/A356752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (k+1)^(k-1) *
 *            |Stirling1(n-2*k,k)|/(2^k * (n-2*k)!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

/* This implementation evaluates the sum directly using unsigned Stirling numbers of the first kind. */

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0) return (seq_t)1;

    idx_t maxK = n / 3;                 /* the largest k for which n-2*k >= k */
    size_t sz = (size_t)(n + 1);        /* array size needed for Stirling rows */

    /* Allocate rows for Stirling numbers of the first kind */
    seq_t *st_prev = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *st_curr = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!st_prev || !st_curr) return (seq_t)0;    /* allocation failure */

    /* Initial row (k = 0): S1(0,0) = 1, others zero (calloc already zeroed) */
    st_prev[0] = (seq_t)1;

    seq_t result = (seq_t)0;
    seq_t pow2 = (seq_t)2;          /* 2^k for current k, start with k = 1 */

    for (idx_t k = 1; k <= maxK; ++k)
    {
        /* Build Stirling row for current k:
           For m < k the value is zero; for m >= k use
               S1(m,k) = S1(m-1,k-1) + (m-1)*S1(m-1,k)
           where S1(m-1,k-1) is in st_prev[m-1] and
                 S1(m-1,k)   is in st_curr[m-1] (from the current row being built). */
        for (idx_t m = 0; m < k; ++m) st_curr[m] = (seq_t)0;        /* zero prefix */

        for (idx_t m = k; m <= n; ++m)
        {
            seq_t t1 = st_prev[m - 1];
            seq_t t2 = st_curr[m - 1];
            t2 = t2 * (seq_t)(m - 1);
            st_curr[m] = t1 + t2;
        }

        /* m = n - 2*k is the index where we need the Stirling number */
        idx_t m_index = n - 2 * k;

        if (m_index >= k)          /* Stirling value exists */
        {
            seq_t stir_val = st_curr[m_index];
            if (stir_val != (seq_t)0)
            {
                /* product (m_index+1)*...*n = n! / m_index! */
                seq_t ratio = (seq_t)1;
                for (idx_t i = m_index + 1; i <= n; ++i)
                    ratio *= (seq_t)i;

                seq_t pow_kplus1 = big_pow((seq_t)(k + 1), (seq_t)(k - 1));
                seq_t term = ratio;
                term *= stir_val;
                term *= pow_kplus1;
                term /= pow2;            /* divide by 2^k */
                result += term;
            }
        }

        /* Prepare for next k: st_curr becomes the previous row */
        seq_t *tmp = st_prev;
        st_prev = st_curr;
        st_curr = tmp;

        pow2 = pow2 * (seq_t)2;        /* update 2^k for the next iteration */
    }

    free(st_prev);
    free(st_curr);
    return result;
}
