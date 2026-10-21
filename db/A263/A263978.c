/*
 * A263978  Least prime p such that n^2 + p^2 is prime, or 0 if none.
 * https://oeis.org/A263978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that n^2 + p^2 is prime, or 0 if none
 * Range:     u8 n <= 15, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static uint64_t *primes = NULL;   /* primes array */
    static size_t primes_cnt = 0;     /* number of primes */

    /* initialise the prime list once */
    if (!primes)
    {
        size_t cnt;
        /* 2 M is more than enough for n≤10 000 expected workload */
        build_primes_upto(2000000, &primes, &cnt);
        primes_cnt = cnt;
    }

    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;

    /* Odd n: only p=2 can give a prime; test it */
    if (n & 1ULL)
    {
        seq_t m = n2 + (seq_t)4;   /* n^2 + 2^2 */
        return is_prime_big(m) ? (seq_t)2 : (seq_t)0;
    }

    /* Even n: search primes in increasing order (skip 2) */
    for (size_t i = 1; i < primes_cnt; ++i)   /* primes[0]==2 is skipped */
    {
        uint64_t p  = primes[i];
        seq_t mp    = n2 + (seq_t)p * (seq_t)p;
        if (is_prime_big(mp))
            return (seq_t)p;
    }

    /* If the pregenerated list exhausted, extend search on the fly */
    uint64_t candidate = (primes_cnt ? primes[primes_cnt-1] : 3) + 2;
    if (candidate <= 3) candidate = 5;      /* ensure odd, >2 */
    while (true)
    {
        if (is_prime_big(candidate))
        {
            seq_t mp = n2 + (seq_t)candidate * (seq_t)candidate;
            if (is_prime_big(mp))
                return (seq_t)candidate;
        }
        candidate += 2;   /* only odd numbers are prime candidates */
    }
}
