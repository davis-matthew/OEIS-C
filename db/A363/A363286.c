/*
 * A363286  Odd primes p such that the congruence 2^x == 1 (mod p) has no
 *          solution for 0 < x < (p - 1)/2.
 * https://oeis.org/A363286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   build_primes_upto, ensure_capacity, free_seq_array, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n-th odd prime p such that 2^x ≡ 1 (mod p) has
 * no solution for 0 < x < (p-1)/2.  Equivalently, the multiplicative
 * order of 2 modulo p is either p-1 or (p-1)/2.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ensure_capacity.h"
#include "../utils/free_seq_array.h"
#include "../utils/mod_pow.h"

static bool
qualifies(uint64_t p, const uint64_t *prime_arr, size_t prime_cnt)
{
    // Compute the multiplicative order of 2 modulo p
    uint64_t order = p - 1;
    for (size_t i = 0; i < prime_cnt && prime_arr[i] * prime_arr[i] <= order; ++i)
    {
        uint64_t q = prime_arr[i];
        if (order % q == 0)
        {
            while (order % q == 0)
            {
                uint64_t temp = order / q;
                if (mod_pow((seq_t)2, temp, (seq_t)p) == (seq_t)1)
                    order = temp;
                else
                    break;
            }
        }
    }
    uint64_t half = (p - 1) / 2;
    return (order == half) || (order == p - 1);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* no value for offset 0, return 0 */

    /* Static state to keep partial results between calls */
    static seq_t        *seq = NULL;      /* found sequence terms          */
    static size_t        seq_cnt = 0;      /* number of terms stored        */
    static size_t        seq_cap = 0;      /* current capacity              */

    static uint64_t      *primes = NULL;   /* array of all primes found     */
    static size_t        prime_cnt = 0;    /* count of primes                */
    static idx_t         sieve_limit = 0;  /* high water mark for sieve      */
    static size_t        next_prime_idx = 0;/* index of next prime to test   */

    /* Grow the sequence until we have n terms */
    while (seq_cnt < (size_t)n)
    {
        /* Ensure we still have a prime list covering the next prime */
        if (next_prime_idx >= prime_cnt)
        {
            /* New sieve boundary */
            if (sieve_limit == 0)
                sieve_limit = 100;             /* minimal base */
            else
                sieve_limit *= 2;              /* enlarge */

            /* Rebuild the prime list up to the new limit */
            if (primes != NULL)
                free_seq_array((seq_t*)primes);
            build_primes_upto(sieve_limit, &primes, &prime_cnt);
            /* next_prime_idx remains valid – the new array contains
               exactly the same primes in the same order as before. */
        }

        uint64_t p = primes[next_prime_idx++];
        if (p == 2) continue;                 /* we want odd primes only */

        if (qualifies(p, primes, prime_cnt))
        {
            ensure_capacity(seq_cnt + 1, &seq_cap, (void**)&seq, sizeof(seq_t));
            seq[seq_cnt++] = (seq_t)p;
        }
    }

    return seq[n - 1];
}
