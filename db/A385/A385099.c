/*
 * A385099  LCM of the denominators of the terms of the n-th row of the
 *          triangle defined by T(n,k) =
 *          (prime(n)-prime(k))/(prime(n)+prime(k)) for k=1 to n-1.
 * https://oeis.org/A385099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm_{1 <= k < n} ((prime(n)+prime(k)) /
 *            gcd(prime(n)-prime(k), prime(n)+prime(k)))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static uint32_t *prime_cache = NULL;   /* array of primes, 1‑based indexing */
static size_t prime_cache_cnt   = 0;
static size_t prime_cache_cap   = 0;
static uint32_t next_candidate  = 2;   /* first number to test for primality */

/* Return the k‑th prime (k >= 1).  Generates primes incrementally
   and stores them in prime_cache.  */
static uint32_t nth_prime(idx_t k)
{
    if (k == 0) return 0;   /* never used, safety */

    while (prime_cache_cnt < (size_t)k)
    {
        uint32_t p = next_candidate;

        /* test p for primality using primes already in cache */
        bool is_prime = true;
        for (size_t i = 0; i < prime_cache_cnt; ++i)
        {
            uint32_t q = prime_cache[i];
            if ((uint64_t)q * q > p) break;
            if (p % q == 0)
            {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
        {
            /* grow cache if necessary */
            if (prime_cache_cnt >= prime_cache_cap)
            {
                size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 256;
                uint32_t *tmp = (uint32_t *)realloc(prime_cache, new_cap * sizeof(uint32_t));
                if (!tmp) abort();
                prime_cache = tmp;
                prime_cache_cap = new_cap;
            }
            prime_cache[prime_cache_cnt++] = p;
        }

        /* advance to next candidate number: after 2 only odd numbers */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }
    return prime_cache[k - 1];
}

/* Compute a(n) as defined in the OEIS entry.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range, return 0 */

    seq_t lcm = 1;          /* lcm of reduced denominators */

    if (n > 1)
    {
        uint32_t p_n = nth_prime(n);   /* prime(n) */

        for (idx_t k = 1; k < n; ++k)
        {
            uint32_t p_k = nth_prime(k);   /* prime(k) */

            /* Numerator and denominator of T(n,k) */
            seq_t num = (seq_t)p_n - (seq_t)p_k;
            seq_t den = (seq_t)p_n + (seq_t)p_k;

            /* Reduce the fraction */
            seq_t g  = gcd(num, den);   /* gcd(den, num) is the same */
            seq_t d  = den / g;         /* reduced denominator */

            /* Update the running LCM: lcm = lcm / gcd(lcm,d) * d */
            seq_t g2 = gcd(lcm, d);
            lcm = (lcm / g2) * d;
        }
    }

    return lcm;
}
