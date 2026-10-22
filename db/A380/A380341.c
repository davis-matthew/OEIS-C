/*
 * A380341  a(n) = phi(1 + phi(4 + phi(9 + ... phi(n^2)))).
 * https://oeis.org/A380341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = φ(1 + φ(4 + φ(9 + … φ(n²))))
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Prime table used for computing φ(n) via trial division. */
static uint32_t *prime_table = NULL;
static size_t prime_count = 0;
static int primes_initialized = 0;

/* Initialize the prime table up to a fixed limit (20000). */
static void init_primes(void) {
    if (primes_initialized) return;
    const uint32_t limit = 20000;
    uint8_t *sieve = (uint8_t*)malloc(limit + 1);
    if (!sieve) return;                     /* allocation failed – unlikely */
    memset(sieve, 1, limit + 1);
    sieve[0] = sieve[1] = 0;
    for (uint32_t p = 2; p * p <= limit; ++p) {
        if (sieve[p]) {
            for (uint64_t m = (uint64_t)p * p; m <= limit; m += p)
                sieve[(size_t)m] = 0;
        }
    }
    prime_count = 0;
    for (uint32_t p = 2; p <= limit; ++p)
        if (sieve[p]) ++prime_count;

    prime_table = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t p = 2; p <= limit; ++p) {
        if (sieve[p]) prime_table[idx++] = p;
    }
    free(sieve);
    primes_initialized = 1;
}

/* Compute φ(m) for a number that fits in 64 bits, using the prime table. */
static seq_t phi_u64(uint64_t m) {
    if (m <= 1) return (seq_t)m;          /* φ(1)=1, φ(0) is not needed */
    uint64_t result = m;
    uint64_t temp = m;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_table[i];
        if ((uint64_t)p * (uint64_t)p > temp) break;
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1) {                        /* remaining prime factor */
        result -= result / temp;
    }
    return (seq_t)result;
}

/* Compute the n‑th term of A380341. */
seq_t a(idx_t n) {
    init_primes();                         /* ensure the prime table exists */

    /* Start with φ(n²). */
    uint64_t n64 = (uint64_t)n;
    seq_t val = phi_u64(n64 * n64);

    /* Iterate from n‑1 down to 1, applying φ(current_square + previous). */
    for (idx_t i = n; i > 1; --i) {
        uint64_t sq = (uint64_t)(i - 1u) * (uint64_t)(i - 1u);
        uint64_t inner = sq + (uint64_t)val;
        val = phi_u64(inner);
    }

    return val;
}
