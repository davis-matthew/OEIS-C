/*
 * A245063  Primes p such that (p reversed)-1 is not a prime.
 * https://oeis.org/A245063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 717 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A245063: a(n) = the n-th prime p such that rev(p)-1 is not prime. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/reverse_digits_idx.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1 */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Build the cache on first call (sufficiently large to cover ~10k terms). */
    if (cache == NULL)
    {
        const idx_t LIMIT = 200000;   /* primes up to 200k → >10k qualifying primes */
        size_t i, j;
        /* Simple sieve of Eratosthenes */
        uint8_t *sieve = (uint8_t*)malloc((size_t)(LIMIT + 1));
        if (!sieve) return 0;        /* allocation failure, return 0 */
        for (i = 0; i <= (size_t)LIMIT; ++i) sieve[i] = 1;
        sieve[0] = sieve[1] = 0;
        for (i = 2; i * i <= (size_t)LIMIT; ++i)
            if (sieve[i])
                for (j = i * i; j <= (size_t)LIMIT; j += i)
                    sieve[j] = 0;

        /* allocate initial capacity for the result array */
        cache_cap = 4096;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) { free(sieve); return 0; }   /* allocation failure */

        /* iterate over primes and keep those that satisfy the condition */
        for (i = 2; i <= (size_t)LIMIT; ++i)
        {
            if (!sieve[i]) continue;      /* not prime */
            /* reverse the prime and test rev(p)-1 */
            idx_t rev_val = reverse_digits_idx(i);
            idx_t rev_minus1 = rev_val - 1;
            if (!isPrimeGeneric((seq_t)rev_minus1))
            {
                /* store prime into cache */
                if (cache_len == cache_cap)
                {
                    cache_cap *= 2;
                    seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                    if (!tmp) { free(sieve); mem_free(cache); return 0; }
                    cache = tmp;
                }
                cache[cache_len++] = (seq_t)i;
            }
        }

        free(sieve);
    }

    /* Return the requested term, if available. */
    if (n <= (idx_t)cache_len)
        return cache[n - 1];
    else
        return 0;   /* out of range (should not happen for n ≤ 10000) */
}
