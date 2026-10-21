/*
 * A272863  Numerator of the ratio of consecutive prime gaps.
 * https://oeis.org/A272863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator((prime(n+2)-prime(n+1))/(prime(n+1)-prime(n)))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cached list of primes (0‑based index).  prime_cache[i] = i‑th prime (p₁ = 2). */
static uint64_t *prime_cache   = NULL;
static size_t   prime_cache_len = 0;   /* number of primes stored */
static size_t   prime_cache_cap = 0;   /* allocated capacity */

/* Euclidean GCD for 64‑bit unsigned integers. */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Ensure that prime_cache contains primes up to index idx (0‑based). */
static void ensure_primes(idx_t idx)
{
    /* Allocate or grow the cache as needed. */
    while (prime_cache_len <= (size_t)idx) {
        uint64_t candidate = (prime_cache_len == 0) ? 2 : prime_cache[prime_cache_len - 1] + 1;
        bool ok;

        /* Find the next prime by trial division. */
        do {
            ok = true;
            for (size_t i = 0; i < prime_cache_len; ++i) {
                uint64_t p = prime_cache[i];
                if ((unsigned __int128)p * p > candidate)
                    break;
                if (candidate % p == 0) {
                    ok = false;
                    break;
                }
            }
            if (!ok)
                ++candidate;
        } while (!ok);

        /* Grow storage if necessary. */
        if (prime_cache_len == prime_cache_cap) {
            size_t new_cap = (prime_cache_cap == 0) ? 64 : prime_cache_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
            if (!tmp)
                abort();  /* allocation failed; terminate */
            prime_cache = tmp;
            prime_cache_cap = new_cap;
        }

        prime_cache[prime_cache_len++] = candidate;
    }
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for offset 1; return 0 as a safe fallback */
        return (seq_t)0;

    /* We need primes pₙ, pₙ₊₁, pₙ₊₂ → indices n‑1, n, n+1 (0‑based). */
    ensure_primes((size_t)(n + 1));

    uint64_t p_n   = prime_cache[(size_t)(n - 1)];
    uint64_t p_n1  = prime_cache[(size_t)n];
    uint64_t p_n2  = prime_cache[(size_t)(n + 1)];

    uint64_t d1 = p_n1 - p_n;   /* gap at position n   */
    uint64_t d2 = p_n2 - p_n1;  /* gap at position n+1 */

    uint64_t g = gcd_u64(d1, d2);
    seq_t result = (seq_t)(d2 / g);   /* numerator after reduction */
    return result;
}
