/*
 * A380354  a(n) = phi(2 + phi(3 + phi(5 + ... + phi(prime(n))))), where phi
 *          is Euler totient function (A000010).
 * https://oeis.org/A380354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(2 + phi(3 + phi(5 + ... + phi(prime(n))))), with phi
 *            = Euler totient (A000010)
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation generates primes on demand and calculates
   the Totient of an arbitrary seq_t value using only the prime
   list.  No external helper functions are required. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Shared prime cache */
static uint64_t *prime_cache = NULL;   /* array of primes */
static size_t     prime_cnt  = 0;      /* number of primes stored */
static size_t     prime_cap  = 0;      /* allocated capacity */

/* Ensure that at least 'n' primes are stored in prime_cache.       */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;

    if (!prime_cache) {
        prime_cap = 128;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_cache)
            exit(EXIT_FAILURE);
    }

    uint64_t candidate = (prime_cnt == 0) ? 2 : prime_cache[prime_cnt - 1] + 1;
    while (prime_cnt < (size_t)n) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_cache[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_cache,
                                                    prime_cap * sizeof(uint64_t));
                if (!tmp)
                    exit(EXIT_FAILURE);
                prime_cache = tmp;
            }
            prime_cache[prime_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* Compute Euler's Totient for a seq_t value using the cached primes. */
static seq_t seq_totient(seq_t n)
{
    seq_t result = n;
    seq_t temp   = n;
    size_t i = 0;
    while (i < prime_cnt) {
        seq_t p  = (seq_t)prime_cache[i];
        seq_t p2 = p * p;
        if (p2 > temp)
            break;
        if (temp % p == 0) {
            while (temp % p == 0)
                temp /= p;
            result -= result / p;
        }
        ++i;
    }
    if (temp > (seq_t)1)
        result -= result / temp;
    return result;
}

/* Main sequence function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* undefined, but safe */

    /* Ensure we have the first n primes. */
    ensure_primes(n);

    /* Innermost phi(prime(n)). */
    seq_t current = seq_totient((seq_t)prime_cache[n - 1]);

    /* Iterate outward over primes n-1 down to 1. */
    for (idx_t k = n - 1; k > 0; --k) {
        seq_t sum = (seq_t)prime_cache[k - 1] + current;
        current  = seq_totient(sum);
    }

    return current;
}
