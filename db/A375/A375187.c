/*
 * A375187  Number of unitary square divisors of n!.
 * https://oeis.org/A375187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^k where k = #{p <= n | v_p(n!) is even} (number of
 *            unitary square divisors of n!) a(0) = 1 (0! = 1 has no prime
 *            factors)
 * Range:     u8 n <= 47, u16 n <= 177, u32 n <= 451, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the factorial has no prime factors,
       thus k = 0 and a(n) = 1. */
    if (n <= 1)
        return (seq_t)1;

    idx_t size = n + 1;
    /* Allocate a sieve array: 0 = prime, 1 = composite. */
    unsigned char *sieve = (unsigned char *)calloc((size_t)size, 1);
    if (!sieve) /* allocation failure – return 0 as safety */
        return (seq_t)0;

    sieve[0] = 1; /* 0 is not prime */
    if (size > 1)
        sieve[1] = 1; /* 1 is not prime */

    /* Sieve of Eratosthenes up to n. */
    for (idx_t p = 2; p * p <= n; ++p) {
        if (!sieve[p]) {
            for (idx_t mult = p * p; mult <= n; mult += p)
                sieve[mult] = 1;
        }
    }

    /* Count primes with even exponent in n!. */
    idx_t evenCount = 0;
    for (idx_t p = 2; p <= n; ++p) {
        if (!sieve[p]) {
            idx_t t = n / p;
            int parity = 0;
            while (t) {
                parity ^= (int)(t & 1);
                t /= p;
                /* The remaining t will be 0 eventually. */
            }
            if (parity == 0)
                ++evenCount;
        }
    }
    free(sieve);

    /* Compute 2^evenCount using left‑shift; works for any seq_t. */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < evenCount; ++i)
        result <<= 1;
    return result;
}
