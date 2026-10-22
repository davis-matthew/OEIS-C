/*
 * A343900  a(n) = Sum_{k=0..n} (k!)^(k+1) * binomial(n,k).
 * https://oeis.org/A343900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k!)^(k+1) * binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   binomial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t fact = 1;           /* fact = k! for current k (k starts at 0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* (k!)^(k+1)  */
        seq_t pow_k = seq_pow(fact, k + 1);

        /* binomial coefficient C(n,k) */
        seq_t bin = binomial(n, k);

        /* term = (k!)^(k+1) * C(n,k) */
        seq_t term = seq_mul(pow_k, bin);

        /* accumulate towards the sum */
        sum += term;

        /* update factorial for next iteration: (k+1)! = k! * (k+1) */
        if (k != n)
            fact = seq_mul(fact, (seq_t)(k + 1));
    }

    return sum;
}
