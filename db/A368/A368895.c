/*
 * A368895  a(n) = Sum_{k=0..floor(n/3)} (-n)^k * binomial(n-2*k,k).
 * https://oeis.org/A368895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (-n)^k * binomial(n-2*k, k)
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 20, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t power = 1;                /* (-n)^k for current k */
    seq_t nn    = (seq_t)n;         /* cast n to seq_t once */
    idx_t max_k = n / 3;            /* floor(n/3) */
    for (idx_t k = 0; k <= max_k; ++k)
    {
        seq_t comb = binomial(n - 2 * k, k);
        sum += power * comb;
        if (k < max_k)             /* update power for next k */
            power *= -nn;
    }
    return sum;
}
