/*
 * A372250  E.g.f. A(x) satisfies A(x) = exp( x * A(x) * (1 + x^2 * A(x)) ).
 * https://oeis.org/A372250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (n-k+1)^(n-2k-1) / (k! *
 *            (n-3k)!)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the offset n = 0 separately to avoid negative exponents. */
    if (n == 0u)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t T = 1;               /* T_k = n! / (k! * (n-3k)!)      */
    idx_t kmax = n / 3u;       /* floor(n/3)                      */

    for (idx_t k = 0; k <= kmax; ++k)
    {
        /* Update T for k > 0 using the recurrence:
           T_{k+1} = T_k * (n-3k)(n-3k-1)(n-3k-2) / (k+1)            */
        if (k > 0u)
        {
            idx_t prev = k - 1u;
            idx_t prod = n - 3u * prev;             /* n-3(k-1)          */
            seq_t f1 = (seq_t)prod;
            seq_t f2 = (seq_t)(prod - 1u);
            seq_t f3 = (seq_t)(prod - 2u);

            T = T * f1 * f2 * f3;
            T = T / (seq_t)k;                       /* divide by current k */
        }

        /* Compute the power term (n-k+1)^(n-2k-1). */
        seq_t base = (seq_t)(n - k + 1u);
        seq_t exp  = (seq_t)(n - 2u * k - 1u);
        seq_t power = big_pow(base, exp);

        /* Accumulate the contribution for this k. */
        sum += T * power;
    }

    return sum;
}
