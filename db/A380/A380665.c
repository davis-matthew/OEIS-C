/*
 * A380665  Expansion of e.g.f. (1/x) * Series_Reversion( x * (1 - x)^2 *
 *          exp(-x/(1 - x)) ).
 * https://oeis.org/A380665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n+1)^(k-1) * binomial(3n+1, n-k) / k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t base = (seq_t)(n + 1);
    seq_t pow = pow_seq(base, (idx_t)(n - 1));   /* (n+1)^(n-1) for k=n  */
    seq_t ratio = 1;                            /* product_{i=k+1}^{n} i */

    for (idx_t k = n; k >= 1; --k)
    {
        seq_t bin = binomial(3 * n + 1, n - k);
        seq_t term = bin * pow;
        term *= ratio;
        sum += term;

        ratio *= k;                 /* prepare product for next lower k */
        pow /= base;                /* (n+1)^(k-2) for next iteration */
    }

    /* k = 0 term : binomial(3n+1, n) * n! / (n+1) */
    seq_t bin0 = binomial(3 * n + 1, n);
    seq_t term0 = bin0 * ratio;
    term0 /= (seq_t)(n + 1);
    sum += term0;

    return sum;
}
