/*
 * A363479  E.g.f. satisfies A(x) = exp(x * A(x)^3 * (1 + x * A(x)^3)).
 * https://oeis.org/A363479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (3*n+1)^(k-1) * binomial(k,n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A363479. */
#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;               /* a(0) = 1 */

    seq_t fac_n = factorial(n);         /* n! */
    seq_t result = 0;

    seq_t fac_k = 1;                     /* k! for k=0 */
    seq_t pow_val = 1;                   /* (3n+1)^(k-1) for k=1 */

    seq_t base = (seq_t)3 * (seq_t)n + 1;/* 3*n + 1 */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t bin = binomial(k, n - k);
        if (bin == 0) {
            /* Update factorial and power for next iteration */
            fac_k *= (k + 1);
            pow_val *= base;
            continue;
        }

        /* coeff = bin * n! / k! */
        seq_t coeff = bin * fac_n;
        coeff = coeff / fac_k;          /* exact division */

        seq_t term = coeff * pow_val;
        result += term;

        /* Prepare for next k */
        fac_k *= (k + 1);               /* now holds (k+1)! */
        pow_val *= base;                /* exponent increments by 1 */
    }

    return result;
}
