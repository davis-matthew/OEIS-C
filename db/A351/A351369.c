/*
 * A351369  a(n) = Sum_{p|n, p prime} p * prime(p).
 * https://oeis.org/A351369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} p * prime(p)
 * Range:     u8 n <= 10, u16 n <= 112, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses get_prime(p) to obtain the p‑th prime and factor_u64 to factor n. */
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n is at least 1 by specification.  a(1) = 0. */
    if (n == 1)
        return (seq_t)0;

    /* Factor n into its distinct primes.  The largest number of distinct
       prime factors of any 64‑bit integer is less than 64, so a stack
       array of that size is sufficient. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        seq_t prime_of_p = (seq_t)get_prime((idx_t)p);   /* p‑th prime */
        sum += (seq_t)p * prime_of_p;
    }

    return sum;
}
