/*
 * A396842  a(n) = H(2*n)/H(n)^2, where H(n) = Product_{k=0..n} k^k is the
 *          hyperfactorial of n (A002109).
 * https://oeis.org/A396842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=1..n} (n+k)^(n+k) / k^k
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"

/* a(n) = H(2n) / H(n)^2  (H(n) = Π_{k=1}^n k^k) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* limit for prime sieve: 2*n */
    uint64_t limit = (uint64_t)n * 2;
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    seq_t result = (seq_t)1;
    for (size_t idx = 0; idx < prime_cnt; ++idx)
    {
        uint64_t p = primes[idx];
        uint64_t e = 0;
        uint64_t power = p;
        while (power <= limit)
        {
            uint64_t t2 = ((uint64_t)n * 2) / power;
            uint64_t t1 = (uint64_t)n / power;
            __int128 s2 = (__int128)power * t2 * (t2 + 1) / 2;
            __int128 s1 = (__int128)power * t1 * (t1 + 1) / 2;
            e += (uint64_t)(s2 - 2 * s1);
            if (power > limit / p)   /* avoid overflow on next multiply */
                break;
            power *= p;
        }
        seq_t pow_val = big_pow((seq_t)p, (seq_t)e);
        result = result * pow_val;
    }
    free(primes);
    return result;
}
