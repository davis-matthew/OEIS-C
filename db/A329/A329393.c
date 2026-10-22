/*
 * A329393  Number of odd divisors minus number of even divisors of the n-th
 *          composite.
 * https://oeis.org/A329393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = τ(c_n) * (1 - k_n), where the n-th composite m_n is
 *            written as m_n = 2^k_n * c_n with c_n odd, and τ(c_n) is the
 *            number of divisors of c_n. This equals the number of odd
 *            divisors minus the number of even divisors of m_n
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static storage for already generated data */
    static uint64_t *composites = NULL;
    static seq_t   *diffs      = NULL;
    static size_t   composites_size = 0;
    static size_t   composites_cap  = 0;
    static uint64_t *primes = NULL;
    static size_t   primes_size = 0;
    static size_t   primes_cap  = 0;
    static uint64_t current_candidate = 4;   /* next number to test for primality */

    /* Initialise prime list with 2 and 3 if not already done */
    if (!primes) {
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) return (seq_t)0;
        primes[0] = 2;
        primes[1] = 3;
        primes_size = 2;
    }

    /* Expand cached data until we have at least n composites */
    while (composites_size < n) {
        /* Find the next composite number */
        for (;;) {
            /* Primality test using stored primes up to sqrt(current_candidate).
               No floating‑point operations are used. */
            bool is_prime = true;
            for (size_t i = 0; i < primes_size; ++i) {
                uint64_t p = primes[i];
                if (p > current_candidate / p) break;   /* p*p > current_candidate */
                if (current_candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (!is_prime) break;            /* composite found */

            /* current_candidate is prime, add it to the prime list */
            if (primes_size == primes_cap) {
                size_t new_cap = primes_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                if (!tmp) return (seq_t)0;
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_size++] = current_candidate;
            ++current_candidate;
        }

        uint64_t m = current_candidate;

        /* Resize composite and diff arrays if necessary */
        if (composites_size == composites_cap) {
            size_t new_cap = composites_cap ? composites_cap * 2 : 16;
            uint64_t *tmpc = (uint64_t *)realloc(composites, new_cap * sizeof(uint64_t));
            seq_t *tmpd = (seq_t *)realloc(diffs, new_cap * sizeof(seq_t));
            if (!tmpc || !tmpd) return (seq_t)0;
            composites = tmpc;
            diffs = tmpd;
            composites_cap = new_cap;
        }

        composites[composites_size] = m;

        /* Separate powers of two from the odd part */
        uint64_t odd_part = m;
        size_t k = 0;
        while ((odd_part & 1ULL) == 0) {
            odd_part >>= 1;
            ++k;
        }

        /* Compute τ(odd_part) using trial division with known primes. */
        seq_t tau = 1;
        uint64_t temp = odd_part;
        for (size_t i = 0; i < primes_size && primes[i] <= temp / primes[i]; ++i) {
            uint64_t p = primes[i];
            if (temp % p == 0) {
                size_t exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                tau *= (seq_t)(exp + 1);
            }
        }
        if (temp > 1) tau *= (seq_t)2;   /* remaining prime factor */

        diffs[composites_size] = tau * ((seq_t)1 - (seq_t)k);

        ++composites_size;
        ++current_candidate;
    }

    return diffs[n - 1];
}
