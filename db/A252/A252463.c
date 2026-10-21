/*
 * A252463  Hybrid shift: a(1) = 1, a(2n) = n, a(2n+1) = A064989(2n+1); shift
 *          the even numbers one bit right, shift the prime factorization of
 *          odd numbers one step towards smaller primes.
 * https://oeis.org/A252463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1) = 1, a(2n) = n, a(2n+1) = ∏p^e||2n+1 prev_prime(p)^e
 * Range:     u8 n <= 261, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   prev_prime, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prev_prime.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 1u)
        return (seq_t)1;

    /* Even index – just halve the index. */
    if ((n & 1u) == 0u)
    {
        return (seq_t)n >> 1;
    }

    /* Odd index > 1 – shift prime factorisation to the previous prime. */
    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Trial division starting with the smallest odd prime. */
    uint64_t p = 3u;
    while (p <= m / p)
    {
        if (m % p == 0u)
        {
            uint64_t exp = 0u;
            while (m % p == 0u)
            {
                m /= p;
                ++exp;
            }
            /* Compute prev_prime(p)^exp and multiply. */
            seq_t base = (seq_t)prev_prime((idx_t)p);
            seq_t term = seq_pow(base, (idx_t)exp);
            result = seq_mul(result, term);
        }
        p += 2u;        /* skip even numbers */
    }

    if (m > 1u)        /* remaining prime factor */
    {
        seq_t base = (seq_t)prev_prime((idx_t)m);
        result = seq_mul(result, base);   /* exp = 1 */
    }

    return result;
}
