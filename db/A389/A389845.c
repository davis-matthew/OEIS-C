/*
 * A389845  E.g.f. A(x) satisfies A(x) = exp(x^3 * A(x) / (1-x)^2).
 * https://oeis.org/A389845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/3⌋} (k+1)^(k-1) * binomial(n-k-1,
 *            n-3*k) / k!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A389845: a(0)=1, a(1)=0, a(2)=0, … */
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Special case */
    if (n == 0) {
        return (seq_t)1;
    }

    /* compute floor(n/3) */
    idx_t max_k = n / 3;
    if (max_k == 0) {
        return (seq_t)0;
    }

    seq_t sum   = (seq_t)0;
    seq_t ratio = (seq_t)1;

    /* Initialize ratio = product_{j=max_k+1}^{n} j  == n! / max_k! */
    for (idx_t j = max_k + 1; j <= n; ++j) {
        ratio *= (seq_t)j;
    }

    /* Iterate k from max_k down to 1 */
    for (idx_t k = max_k; k >= 1; --k) {
        /* binomial(n-k-1, n-3k) */
        seq_t binom = binomial(n - k - 1, n - 3 * k);

        /* (k+1)^(k-1) */
        seq_t pow = seq_pow((seq_t)(k + 1), (idx_t)(k - 1));

        /* term = ratio * binom * pow = (n! / k!) * binom * pow */
        seq_t term = ratio * binom;
        term *= pow;

        sum += term;

        /* Prepare ratio for next k-1: ratio = (k-1)!?   ratio_{k-1}=k * ratio_k */
        if (k > 1) {                /* avoid overflow when k==1 -> end loop */
            ratio *= (seq_t)k;
        }
    }

    return sum;
}
