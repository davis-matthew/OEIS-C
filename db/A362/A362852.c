/*
 * A362852  The number of divisors of n that are both bi-unitary and
 *          exponential.
 * https://oeis.org/A362852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e of n of d(e) if e is odd,
 *            otherwise d(e)-1; where d(k) is the number of divisors of k
 *            (A000005)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Use the helper that factorizes u64 numbers.  */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/*  Count the divisors of an integer k.  k is the exponent of a prime
    factor, so it is small (≤ 64 for 64-bit n).  */
static seq_t divisor_count_u64(uint64_t k)
{
    seq_t count = 1;
    for (uint64_t p = 2; p * p <= k; ++p)
    {
        if (k % p == 0)
        {
            uint64_t exp = 0;
            while (k % p == 0)
            {
                k /= p;
                ++exp;
            }
            count *= (seq_t)(exp + 1);
        }
    }
    if (k > 1)
        count *= (seq_t)2;
    return count;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Enough space for prime factors of numbers up to 2^64-1.  */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = exps[i];
        seq_t d = divisor_count_u64(e);
        if ((e & 1) == 0)   /* e even  */
            d -= (seq_t)1;
        prod *= d;
    }
    return prod;
}
