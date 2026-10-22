/*
 * A387467  a(n) = Sum_{k=0..n} (1-n*i)^k * (1+n*i)^(n-k) * binomial(n,k)^2,
 *          where i is the imaginary unit.
 * https://oeis.org/A387467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} (n^2+1)^k * 2^(n-2k) * binomial(n,2k) *
 *            binomial(2k,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result    = 0;

    seq_t n_sq      = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t pow_base  = n_sq + (seq_t)1;              /* n^2+1 */
    seq_t base_pow  = (seq_t)1;                     /* (n^2+1)^k, start with 1 (k=0) */
    seq_t pow2      = seq_pow((seq_t)2, n);         /* 2^n */
    idx_t max_k     = n / 2;                        /* floor(n/2) */

    for (idx_t k = 0; k <= max_k; ++k)
    {
        if (k > 0)
        {
            base_pow = base_pow * pow_base;         /* multiply by (n^2+1) */
            pow2     = pow2 / ((seq_t)4);           /* divide by 4 to get 2^(n-2k) */
        }

        seq_t term = base_pow;
        term = term * pow2;
        term = term * binomial(n, 2 * k);
        term = term * binomial(2 * k, k);

        result = result + term;
    }

    return result;
}
