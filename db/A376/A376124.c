/*
 * A376124  E.g.f. A(x) satisfies A(x) = x * exp(A(x)) * (1 + 3*A(x)).
 * https://oeis.org/A376124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A376124: a(n) = n! * Sum_{k=1..n} 3^(n-k) * n^(k-1) * binomial(n-1,k-1)/k! */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* n! */
    seq_t n_fact = 1;
    for (idx_t i = 1; i <= n; ++i) n_fact *= (seq_t)i;

    /* initialise terms */
    seq_t sum    = 0;
    seq_t n_pow  = 1;                       /* n^(k-1) */
    seq_t tri_pow = big_pow((seq_t)3, (seq_t)(n - 1));  /* 3^(n-k) */
    seq_t ratio = n_fact;                    /* n! / k!  (k=1) */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t binom = binomial(n - 1, k - 1);
        seq_t term = binom * n_pow * tri_pow * ratio;
        sum += term;

        if (k < n) {
            n_pow  *= (seq_t)n;          /* raise n^(k-1) to n^k   */
            tri_pow /= (seq_t)3;         /* reduce 3^(n-k) to 3^(n-k-1) */
            ratio   /= (seq_t)(k + 1);   /* move from n!/k! to n!/(k+1)! */
        }
    }
    return sum;
}
