/*
 * A376458  a(n) = Sum_{k = 0..n} (-1)^(n+k)*binomial(n, k)*binomial(n+k,
 *          k)*A108625(n-1, n-k).
 * https://oeis.org/A376458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 18, i128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A376458
   Formula: a(n) = Sum_{0 <= i <= k <= n} (-1)^k *
           binomial(n , k) * binomial(2*n-k , n-k) *
           binomial(n-1 , i)^2 * binomial(n+k-i-1 , k-i)
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle trivial n=0 case directly */
    if (n == 0)
        return (seq_t)1;

    /* Pre‑compute C(n-1,i) for i=0..n-1 */
    seq_t *choose = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!choose)
        return (seq_t)0;                /* allocation failure – should never occur */

    choose[0] = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
    {
        /* choose[i] = choose[i-1] * (n-1-(i-1)) / i */
        choose[i] = choose[i - 1] *
                    (seq_t)(n - 1 - (i - 1)) /
                    (seq_t)i;
    }

    seq_t total = (seq_t)0;

    /* Initial binomial factors for k = 0 */
    seq_t binom_nk  = (seq_t)1;                 /* C(n,0)      */
    seq_t binom_2nk = binomial(2 * n, n);      /* C(2n,n)     */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Inner sum over i */
        seq_t inner = (seq_t)0;
        idx_t max_i = (k <= n - 1) ? k : (n - 1);

        /* B = C(n+k-1, k)  (i=0 term) */
        seq_t B = binomial(n + k - 1, k);

        for (idx_t i = 0; i <= max_i; ++i)
        {
            /* term = C(n-1,i)^2 * C(n+k-i-1,k-i) */
            seq_t term = choose[i];
            term *= term;          /* choose[i]^2        */
            term *= B;             /* * C(n+k-i-1,k-i) */
            inner += term;

            /* Update B for next i (if any) */
            if (i < max_i)
            {
                seq_t num = (seq_t)(k - i);                 /* r = k-i   */
                seq_t den = (seq_t)(n + k - 1 - i);         /* m = n+k-i-1 */
                B = B * num / den;
            }
        }

        /* Sign factor (-1)^k */
        seq_t term_k = inner * binom_nk * binom_2nk;
        if (k & 1)
            term_k = -term_k;

        total += term_k;

        /* Update binomial factors for next k (if any) */
        if (k < n)
        {
            /* C(n, k+1) */
            binom_nk = binom_nk * (seq_t)(n - k) / (seq_t)(k + 1);

            /* C(2n-(k+1), n-(k+1)) */
            binom_2nk = binom_2nk * (seq_t)(n - k) / (seq_t)(2 * n - k);
        }
    }

    free(choose);
    return total;
}
