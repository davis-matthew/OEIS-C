/*
 * A377810  E.g.f. satisfies A(x) = exp(x * A(x)) / (1 - x)^2.
 * https://oeis.org/A377810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (k+1)^(k-1) * binomial(n+k+1, n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* compute factorial(n) */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    seq_t sum = 0;
    seq_t f = fact;                /* factorial(n)/k!  (k starts at 0) */
    seq_t B = (seq_t)(n + 1);       /* binomial(n+1, n)  (B_0) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t p;
        if (k == 0) {
            p = 1;                 /* (1)^(-1) is treated as 1 */
        } else {
            p = big_pow((seq_t)(k + 1), (seq_t)(k - 1));
        }

        seq_t term = f * p;
        term = term * B;
        sum += term;

        if (k == n) {
            break;                 /* last term processed */
        }

        /* update factorial part for the next k */
        f /= (seq_t)(k + 1);

        /* update binomial part: B_{k+1} = B_k * (n+k+2)*(n-k) / ((2k+3)*(2k+2)) */
        seq_t num = (seq_t)(n + k + 2) * (seq_t)(n - k);
        seq_t den = (seq_t)(2 * k + 3) * (seq_t)(2 * k + 2);
        B = B * num;
        B = B / den;
    }

    return sum;
}
