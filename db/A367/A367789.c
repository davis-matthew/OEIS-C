/*
 * A367789  E.g.f. satisfies A(x) = exp( x/(1-x)^3 * A(x) ).
 * https://oeis.org/A367789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (k+1)^(k-1) * binomial(n+2*k-1, n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367789 */

/* Dependencies */
#include "../utils/big_pow.h"     /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* Handle off‑by‑offset */
    if (n == 0) return (seq_t)1;

    seq_t result  = 0;      /* Sum accumulator */
    seq_t ratio   = 1;      /* n! / k! for current k (starts with k = n) */

    for (idx_t k = n; ; --k) {
        /* (k+1)^(k-1) */
        seq_t pow_k = 1;
        if (k > 0) {
            pow_k = big_pow((seq_t)(k + 1), (seq_t)(k - 1));
        }

        /* binomial(n + 2k - 1, n - k) */
        seq_t comb = binomial(n + 2 * k - 1, n - k);

        /* term = comb * pow_k * ratio */
        seq_t term = comb;
        term *= pow_k;
        term *= ratio;

        result += term;

        if (k == 0) break;      /* finished loop */
        ratio *= (seq_t)k;       /* update ratio for next k-1 */
    }

    return result;
}
