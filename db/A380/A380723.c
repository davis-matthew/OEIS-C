/*
 * A380723  E.g.f. A(x) satisfies A(x) = exp(x * A(x)^2) / (1 - x*A(x)^2).
 * https://oeis.org/A380723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (2*n+1)^(k-1) * binomial(3*n-k, n-k)/k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* binomial(3*n, n) */
    seq_t B = 1;
    for (idx_t i = 1; i <= n; ++i) {
        B *= (seq_t)(2 * n + i);
        B /= (seq_t)i;
    }

    /* n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    seq_t sum = 0;

    /* k = 0 term: (2*n+1)^(-1) */
    seq_t term = B * fact;
    term /= (seq_t)(2 * n + 1);
    sum += term;

    /* prepare for k = 1 ... n */
    seq_t R = fact;                 /* n! / k!  (currently k = 0) */
    B = B * (seq_t)n / (seq_t)(3 * n);   /* B(1) */
    seq_t pow = 1;                 /* (2*n+1)^(k-1) for k = 1 */

    for (idx_t k = 1; k <= n; ++k) {
        term = B * R * pow;
        sum += term;

        if (k == n) break;

        /* update for next k+1 */
        R /= (seq_t)(k + 1);                         /* n!/(k+1)! */
        B = B * (seq_t)(n - k) / (seq_t)(3 * n - k); /* B(k+1)  */
        pow *= (seq_t)(2 * n + 1);                   /* next pow  */
    }

    return sum;
}
