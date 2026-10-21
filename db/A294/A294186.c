/*
 * A294186  Number of distinct greater twin primes which are in Goldbach
 *          partitions of 2n.
 * https://oeis.org/A294186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct greater twin primes that appear in a
 *            Goldbach partition of 2n. a(n) is defined for n ≥ 1
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* The types idx_t and seq_t are supplied by the build system;
   do not redefine them here. */

/* Computes a(n) for all n ≥ 1.  Each call builds a sieve up to
   2*n, then counts the primes p such that p is a greater twin prime
   (p-2 is prime) and 2*n-p is also prime.  The result is returned
   as a seq_t value. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* values below the offset are unspecified */

    /* Determine the upper bound for the sieve: 2*n.  The size_t
       type is guaranteed to hold 2*n for the relevant problem sizes. */
    size_t limit = (size_t)(2ULL * n);

    /* Allocate a boolean array for primality marking. */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return 0;   /* out‑of‑memory fallback */

    /* Initialize all entries to true (prime). */
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) {
        is_prime[0] = false;
        if (limit >= 1) is_prime[1] = false;
    }

    /* Classic sieve of Eratosthenes. */
    for (size_t p = 2; p * p <= limit; ++p) {
        if (!is_prime[p]) continue;
        for (size_t q = p * p; q <= limit; q += p) {
            is_prime[q] = false;
        }
    }

    /* Count distinct greater twin primes that appear in a Goldbach
       partition of 2*n. */
    seq_t count = 0;
    size_t target = 2 * n;
    for (size_t p = 2; p <= target; ++p) {
        if (!is_prime[p]) continue;

        /* p must be a greater twin prime: p-2 is prime. */
        if (p <= 2) continue;          /* p-2 would not be prime */
        if (!is_prime[p - 2]) continue;

        /* The complementary part of the Goldbach partition must also
           be prime. */
        size_t q = target - p;
        if (q <= limit && is_prime[q]) {
            ++count;
        }
    }

    free(is_prime);
    return count;
}
