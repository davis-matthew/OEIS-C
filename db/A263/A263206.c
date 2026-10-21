/*
 * A263206  Number of primes p with n^2 < prime(p) < (n+2)^2.
 * https://oeis.org/A263206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_composite_odd, set_composite_odd, small_sieve_mark
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_composite_odd.h"
#include "../utils/set_composite_odd.h"
#include "../utils/small_sieve_mark.h"

/* Compute a(n): number of primes p with n^2 < prime(p) < (n+2)^2,
   where prime(p) is the p‑th prime and the index p itself is prime. */

static bool        initialized = false;
static idx_t       max_n_computed = 0;          /* largest n processed so far */
static idx_t       prime_limit   = 0;          /* (max_n_computed+2)^2 */
static size_t      prime_count   = 0;          /* number of primes <= prime_limit */
static uint32_t   *primes         = NULL;       /* primes[i] = (i+1)-th prime       */
static unsigned char *is_prime_index = NULL;  /* 1 if number (i+1) is prime      */
static uint32_t   *prefix          = NULL;     /* prefix sums of is_prime_index    */

seq_t a(idx_t n)
{
    /* rebuild data if necessary */
    if (!initialized || n > max_n_computed)
    {
        if (primes)        free(primes);
        if (is_prime_index) free(is_prime_index);
        if (prefix)        free(prefix);

        /* new limits */
        idx_t t = n + 2;
        prime_limit = t * t;
        max_n_computed = n;

        /* sieve for primes up to prime_limit */
        size_t sieve_bytes = (prime_limit >> 4) + 1; /* (prime_limit/2+7)/8 */
        unsigned char *sieve = (unsigned char *)calloc(sieve_bytes, 1);
        if (!sieve) exit(1);

        for (idx_t i = 3; i * i <= prime_limit; i += 2)
            if (!is_composite_odd(sieve, i))
                for (idx_t j = i * i; j <= prime_limit; j += i << 1)
                    set_composite_odd(sieve, j);

        /* count primes */
        prime_count = 1; /* prime 2 */
        for (idx_t i = 3; i <= prime_limit; i += 2)
            if (!is_composite_odd(sieve, i))
                ++prime_count;

        /* allocate and fill primes array */
        primes = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
        size_t k = 0;
        primes[k++] = 2;
        for (idx_t i = 3; i <= prime_limit; i += 2)
            if (!is_composite_odd(sieve, i))
                primes[k++] = (uint32_t)i;
        free(sieve);

        /* mark prime indices (numbers <= prime_count) */
        is_prime_index = (unsigned char *)malloc(prime_count * sizeof(unsigned char));
        size_t small_bytes = (prime_count >> 4) + 1;
        unsigned char *small_sieve = (unsigned char *)calloc(small_bytes, 1);
        if (!small_sieve) exit(1);
        small_sieve_mark(prime_count, small_sieve);

        /* handle number 1 specially */
        is_prime_index[0] = 0;                     /* 1 is not prime */
        for (k = 1; k < prime_count; ++k)          /* k+1 runs from 2 ... */
        {
            idx_t num = k + 1;
            if (num == 2)
                is_prime_index[k] = 1;
            else if ((num & 1u) == 0)
                is_prime_index[k] = 0;
            else
                is_prime_index[k] = is_composite_odd(small_sieve, num) ? 0 : 1;
        }
        free(small_sieve);

        /* prefix sum of prime indices */
        prefix = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
        prefix[0] = is_prime_index[0];
        for (k = 1; k < prime_count; ++k)
            prefix[k] = prefix[k - 1] + (is_prime_index[k] ? 1u : 0u);

        initialized = true;
    }

    seq_t lower = (seq_t)n * (seq_t)n;
    idx_t up = n + 2;
    seq_t upper = (seq_t)up * (seq_t)up;

    /* first prime > lower */
    size_t low_idx, high_idx;
    size_t l = 0, r = prime_count;
    while (l < r) {
        size_t m = (l + r) >> 1;
        if ((seq_t)primes[m] <= lower)
            l = m + 1;
        else
            r = m;
    }
    low_idx = l; /* could be prime_count */

    /* last prime < upper */
    l = 0; r = prime_count;
    while (l < r) {
        size_t m = (l + r) >> 1;
        if ((seq_t)primes[m] < upper)
            l = m + 1;
        else
            r = m;
    }
    if (l == 0)
        return 0;
    high_idx = l - 1;

    if (low_idx > high_idx)
        return 0;

    seq_t result = (seq_t)prefix[high_idx];
    if (low_idx > 0)
        result -= (seq_t)prefix[low_idx - 1];
    return result;
}
