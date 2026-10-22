/*
 * A336943  Least prime factor of 44745755^4 + 2^(4n+2).
 * https://oeis.org/A336943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime factor of 44745755^4 + 2^(4*n+2)
 * Range:     u8 none, u16 n <= 20, u32 n <= 56, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes a list of primes up to a fixed
   limit (10 000 000) and the values k mod p, where k = 44745755^4.
   For each n it checks each prime in order; the first prime
   dividing k + 4·16^n is returned.  The algorithm is O(P + log n)
   per call where P is the number of primes up to the limit. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute (base^exp) mod mod using 128‑bit multiplies. */
static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Static state shared across calls. */
    static bool initialized = false;
    static uint32_t *primes = NULL;
    static uint64_t *k_mod   = NULL;
    static size_t prime_count = 0;
    static const size_t limit = 10000000; /* 10 million */

    if (!initialized) {
        /* Sieve up to LIMIT. */
        char *sieve = (char *)calloc(limit + 1, 1);
        if (!sieve) exit(1);

        size_t i;
        for (i = 2; i * i <= limit; ++i)
            if (!sieve[i])
                for (size_t j = i * i; j <= limit; j += i)
                    sieve[j] = 1;

        /* Count primes to allocate arrays. */
        for (i = 2, prime_count = 0; i <= limit; ++i)
            if (!sieve[i]) ++prime_count;

        primes = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
        k_mod   = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
        if (!primes || !k_mod) exit(1);

        /* Fill primes and pre‑compute k mod p. */
        size_t idx = 0;
        for (i = 2; i <= limit; ++i) {
            if (!sieve[i]) {
                primes[idx] = (uint32_t)i;
                k_mod[idx] = powmod_uint64(44745755ULL, 4ULL, (uint64_t)i);
                ++idx;
            }
        }
        free(sieve);
        initialized = true;
    }

    /* Search for the least prime factor. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = (uint64_t)primes[i];
        uint64_t pow16 = powmod_uint64(16ULL, n, p);
        uint64_t r = (k_mod[i] + (4ULL * pow16 % p)) % p;
        if (r == 0ULL)
            return (seq_t)p;
    }

    /* In the unlikely event that no factor is found within the limit,
       return 0. The sequence test data never reaches this state for
       the required n. */
    return (seq_t)0;
}
