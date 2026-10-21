/*
 * A274140  Sum of primes dividing n-th triangular number, counted with
 *          multiplicity.
 * https://oeis.org/A274140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of primes dividing the n-th triangular number,
 *            counted with multiplicity
 * Range:     u8 n <= 22, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Triangular number T_n = n*(n+1)/2 */
    seq_t tn = (seq_t)n * ((seq_t)n + 1) / 2;

    /* For n up to the required limits the value fits in 64‑bit
       (sequence tests use n <= 10000).  We cast to uint64_t for
       simple trial division factorisation. */
    uint64_t t = (uint64_t)tn;
    if (t == 1) return (seq_t)0;          /* T_1 = 1 has no prime factors */

    seq_t sum = 0;                        /* result accumulator */
    /* Trial division: factor t into primes and accumulate p*exp */
    for (uint64_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            int exp = 0;
            while (t % p == 0)
            {
                t /= p;
                ++exp;
            }
            sum += (seq_t)p * (seq_t)exp;
        }
    }
    /* If anything remains, it is a prime factor with exponent 1 */
    if (t > 1) sum += (seq_t)t;

    return sum;
}
