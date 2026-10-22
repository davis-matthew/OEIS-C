/*
 * A377746  E.g.f. satisfies A(x) = exp(x) * (1 + x * A(x)^2)^2.
 * https://oeis.org/A377746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0}^{n} (2k+1)^(n-k-1) * binomial(4k+2,k)/(n-k)!
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t result = 0;
    seq_t P = 1;                     /* falling factorial n!/(n-k)! */
    seq_t binom = 1;                  /* binomial(4k+2,k), start with k=0 */
    for (idx_t k = 0; k < n; ++k) {
        seq_t base = (seq_t)(2*k + 1);
        seq_t exp  = (seq_t)(n - k - 1);
        seq_t power = big_pow(base, exp);
        seq_t term = P * power;
        term = term * binom;
        result += term;

        /* update P for next k */
        P = P * (seq_t)(n - k);

        /* update binom for next k (unless we're at the last iteration) */
        if (k + 1 < n) {
            idx_t kk = k + 1;
            seq_t num1 = (seq_t)(4*kk - 1);
            seq_t num2 = (seq_t)(4*kk);
            seq_t num3 = (seq_t)(4*kk + 1);
            seq_t num4 = (seq_t)(4*kk + 2);
            seq_t num = num1 * num2 * num3 * num4;

            seq_t den1 = (seq_t)(kk);
            seq_t den2 = (seq_t)(3*kk);
            seq_t den3 = (seq_t)(3*kk + 1);
            seq_t den4 = (seq_t)(3*kk + 2);
            seq_t den = den1 * den2 * den3 * den4;

            binom = binom * num;
            binom = binom / den;            /* exact division */
        }
    }

    /* k = n term: binomial(4n+2,n)/(2n+1) * n! */
    seq_t binom_n = binomial(4*n + 2, n);
    seq_t last = binom_n / (seq_t)(2*n + 1);
    last = last * P;                      /* P holds n! after loop */
    result += last;

    return result;
}
