/*
 * A357852  Replace prime(k) with prime(k+2) in the prime factorization of n.
 * https://oeis.org/A357852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A003961(A003961(n)) // replace prime(k) by prime(k+2)
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64, get_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Helpers provided by the framework */
#include "../utils/factor_u64.h"     /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/get_prime.h"      /* uint32_t get_prime(idx_t k); */
#include "../utils/prime_pi.h"       /* seq_t prime_pi(idx_t limit); */
#include "../utils/big_pow.h"        /* seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first term */
    if (n == (idx_t)1) return (seq_t)1;

    /* Factorize the integer n (fits in uint64_t in the target range). */
    uint64_t primes[64];   /* enough capacity for any 64‑bit integer */
    uint64_t exps[64];
    size_t   cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t  p  = primes[i];
        uint32_t  e  = (uint32_t)exps[i];

        /* Index of the prime p (π(p)).  Since p is prime, this is its
           ordinal in the prime sequence. */
        seq_t k_seq = prime_pi(p);
        idx_t  k    = (idx_t)k_seq;

        /* Prime two places after k: prime(k+2). */
        uint32_t newprime = get_prime(k + (idx_t)2);

        /* Compute newprime^e. */
        seq_t prime_pow = big_pow((seq_t)newprime, (seq_t)e);

        /* Accumulate the product. */
        result *= prime_pow;
    }
    return result;
}
