/*
 * A277419  a(n) = n!*LaguerreL(n, -5*n).
 * https://oeis.org/A277419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*LaguerreL(n, -5*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* factorial(n) is needed for the trailing factorial part */
    seq_t fact_n = factorial(n);

    seq_t sum = 0;
    seq_t binom = 1;          /* binom(n,0) */
    seq_t fact_suf = fact_n;  /* (n-k)! , starts with (n-0)! = n!      */
    seq_t pow5 = 1;           /* 5^k, starts with 1 (k=0) */
    seq_t pown = 1;           /* n^k, starts with 1 (k=0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* term_k = binom(n,k)^2 * (n-k)! * 5^k * n^k */
        seq_t term = binom * binom;
        term = term * fact_suf;
        term = term * pow5;
        term = term * pown;
        sum += term;

        if (k == n) break;

        /* update for next k+1 */
        binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
        pow5   = pow5 * 5;
        pown   = pown * n;
        fact_suf = fact_suf / (seq_t)(n - k);
    }

    return sum;
}
