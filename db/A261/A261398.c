/*
 * A261398  Integer coefficients arising from a formula for Sum_{m>=1}
 *          sin(Pi*m/3)^2/m^2.
 * https://oeis.org/A261398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..floor((n-1)/2)} (-1)^i * binomial(n,i) *
 *            (n-2*i)^(n-1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1  (a(1) is the first term) */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

/* External linkage required */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 return 0 safely. */
    if (n == 0)
        return 0;

    /* `max_i` is floor((n-1)/2).  Since n >= 1, (n-1) is non‑negative. */
    const idx_t max_i = (n - 1) / 2;

    seq_t sum = 0;

    for (idx_t i = 0; i <= max_i; ++i)
    {
        /* binomial(n,i) – the utility returns a seq_t. */
        seq_t comb = binomial(n, i);

        /* base = n - 2*i; this is always >= 1 because i <= floor((n-1)/2). */
        const seq_t base = (seq_t)(n - 2 * i);

        /* power = (n-2*i)^(n-1) */
        seq_t pow = seq_pow(base, n - 1);

        /* term = comb * pow */
        seq_t term = comb * pow;

        /* alternating sign */
        if (i & 1)
            sum -= term;
        else
            sum += term;
    }

    return sum;
}
