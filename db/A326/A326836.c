/*
 * A326836  Heinz numbers of integer partitions whose maximum part divides
 *          their sum.
 * https://oeis.org/A326836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: For each integer m ≥ 2 in ascending order,
   factor m into primes using a smallest‑prime‑factor sieve.
   For each prime factor p with exponent e, let idx be the
   index of p in the prime list (prime(1)=2, prime(2)=3, …).
   Compute sum += idx*e and keep track of maxidx = max(idx).
   If maxidx > 0 and sum % maxidx == 0, m is a Heinz number of a
   partition whose maximum part divides its sum, and is stored.
   The function returns the n‑th such number (a(1)=2). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *spf         = NULL;  /* smallest prime factor */
static uint32_t *prime_idx   = NULL;  /* prime → 1‑based index */
static seq_t  *cache         = NULL;  /* cached sequence values  */
static size_t cache_len      = 0;     /* number of cached terms   */
static size_t cache_cap      = 0;     /* capacity of the cache    */
static idx_t   next_candidate = 2;    /* next integer to test     */
static size_t sieve_limit    = 0;     /* current sieve upper bound*/
static bool    initialized   = false;

/* Build an SPF sieve up to and including "limit" (>=2). */
static void build_sieve(size_t limit)
{
    free(spf);
    free(prime_idx);
    spf        = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    prime_idx  = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    for (size_t i = 0; i <= limit; ++i) {
        spf[i]        = 0;
        prime_idx[i]  = 0;
    }

    uint32_t pcnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i]        = (uint32_t)i;
            ++pcnt;
            prime_idx[i]  = pcnt;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
            }
        }
    }
    sieve_limit = limit;
}

/* Ensure that at least "n" terms are present in the cache. */
static void ensure_terms(idx_t n)
{
    if ((idx_t)cache_len >= n)
        return;

    if (!initialized) {
        build_sieve(1000000);   /* sufficient for n≈10 000 */
        initialized = true;
    }

    while ((idx_t)cache_len < n) {
        /* Grow the sieve if the next candidate would exceed it. */
        while ((size_t)next_candidate > sieve_limit) {
            size_t new_limit = sieve_limit * 2;
            if (new_limit < 1000000) new_limit = 1000000;
            build_sieve(new_limit);
        }

        /* Test the current candidate. */
        uint64_t x = next_candidate;
        seq_t sum = 0;
        uint32_t max_idx = 0;

        while (x > 1) {
            uint32_t p   = spf[x];
            uint32_t idx = prime_idx[p];
            int exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            sum += (seq_t)idx * (seq_t)exp;
            if (idx > max_idx)
                max_idx = idx;
        }

        if (max_idx > 0 && (sum % (seq_t)max_idx) == 0) {
            if (cache_len + 1 > cache_cap) {
                cache_cap = (cache_cap == 0) ? 256 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for this OEIS offset */
    ensure_terms(n);
    return cache[n - 1];
}
