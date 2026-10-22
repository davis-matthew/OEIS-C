/*
 * A371068  E.g.f. satisfies A(x) = 1 + x*exp(x^3*A(x)^3).
 * https://oeis.org/A371068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A371068
   Formula: a(n) = n! * Σ_{k=0..⌊n/3⌋} (n-3k)^k * binomial(3k+1, n-3k) / ((3k+1) * k!) */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_k = n / 3;                       /* floor(n/3) */
    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t rest = n - 3 * k;                /* n - 3k */
        seq_t pow_val = seq_pow((seq_t)rest, k);/* (n-3k)^k */
        seq_t binom_val = binomial(3 * k + 1, rest);
        if (binom_val == 0) continue;          /* skip zero terms */

        /* compute product_{i=k+1}^{n} i = n! / k!  */
        seq_t ratio = 1;
        for (idx_t i = k + 1; i <= n; ++i) {
            ratio *= (seq_t)i;
        }
        /* divide by (3k+1) */
        ratio /= (seq_t)(3 * k + 1);

        /* term = binom * pow_val * ratio  */
        seq_t term = binom_val;
        term *= pow_val;
        term *= ratio;
        sum += term;
    }
    return sum;
}
