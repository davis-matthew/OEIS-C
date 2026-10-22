/*
 * A321236  a(n) = Sum_{d|n} mu(d)^2*d^n.
 * https://oeis.org/A321236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{p|n} (1 + p^n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 1. a(1) = 1 and for n = 0 the value is
       irrelevant because the sequence is offset at 1. */
    if (n <= 1) return (seq_t)1;

    idx_t temp   = n;      /* copy of n used for factorisation */
    idx_t orig_n = n;      /* original exponent */
    seq_t result = 1;

    /* Simple trial division factorisation, sufficient for n up to about
       10^4 (and far beyond for most practical tests). */
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            /* compute p^n and add 1 */
            seq_t term = big_pow((seq_t)p, (seq_t)orig_n);
            term = term + 1;
            result = result * term;

            /* strip all factors of p to keep p distinct */
            while (temp % p == 0) temp /= p;
        }
    }

    /* If a prime factor larger than sqrt(n) remains, process it. */
    if (temp > 1)
    {
        seq_t term = big_pow((seq_t)temp, (seq_t)orig_n);
        term = term + 1;
        result = result * term;
    }

    return result;
}
