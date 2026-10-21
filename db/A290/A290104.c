/*
 * A290104  a(n) = A003963(n) / A290103(n).
 * https://oeis.org/A290104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003963(n) / A290103(n) A003963(n): product of the
 *            indices of the prime factors of n (with multiplicity)
 *            A290103(n): lcm of those indices
 * Range:     u8 n <= 3434, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

static bool primes_initialized = false;
static idx_t prime_limit = 0;
static idx_t *primes = NULL;
static size_t primes_count = 0;

/* Simple sieve of Eratosthenes up to N (inclusive).
   After calling, 'primes' will point to an array of all primes <= N,
   and 'primes_count' will contain their count. */
static void
init_primes(idx_t N)
{
    /* free previous allocations */
    if (primes != NULL) {
        free(primes);
        primes = NULL;
    }

    if (N < 2) {
        prime_limit = N;
        primes_count = 0;
        return;
    }

    /* is_prime[i] == 1  iff i is prime  (1‑based) */
    uint8_t *is_prime = (uint8_t *)calloc((size_t)N + 1, sizeof(uint8_t));
    if (!is_prime) return;              /* allocation failure: abort gracefully */

    for (size_t i = 2; i <= (size_t)N; ++i) {
        is_prime[i] = 1;
    }

    for (idx_t i = 2; (idx_t)((uint64_t)i * i) <= N; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= N; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* count primes to allocate array */
    size_t cnt = 0;
    for (idx_t i = 2; i <= N; ++i) {
        if (is_prime[i]) ++cnt;
    }

    primes = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!primes) {
        free(is_prime);
        return;                           /* allocation failure: abort gracefully */
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= N; ++i) {
        if (is_prime[i]) {
            primes[idx++] = i;
        }
    }
    primes_count = cnt;
    prime_limit = N;

    free(is_prime);
}

/* Return the 1‑based index of prime p in the prime list.
   The prime must be in the list (p <= prime_limit). */
static inline idx_t
prime_index_of(idx_t p)
{
    for (size_t i = 0; i < primes_count; ++i) {
        if (primes[i] == p) return (idx_t)(i + 1);
    }
    return 0;  /* should not happen for valid input */
}

/* Least common multiple of two seq_t numbers. */
static seq_t
lcm_seq(seq_t a, seq_t b)
{
    if (a == 0 || b == 0) return 0;
    seq_t g = gcd(a, b);
    return (a / g) * b;
}

seq_t a(idx_t n)
{
    /* Ensure we have primes up to n for factorization and index lookup. */
    if (!primes_initialized || n > prime_limit) {
        init_primes(n);
        primes_initialized = true;
    }

    seq_t product = (seq_t)1;
    seq_t lcm_val = (seq_t)1;

    idx_t temp = n;

    /* trial division by stored primes */
    for (size_t i = 0; i < primes_count && (idx_t)((uint64_t)primes[i] * primes[i]) <= temp; ++i) {
        idx_t p = primes[i];
        if (temp % p == 0) {
            size_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            idx_t idx = prime_index_of(p);
            seq_t idx_seq = (seq_t)idx;

            /* product *= idx_seq^exp */
            for (size_t k = 0; k < exp; ++k) {
                product *= idx_seq;
            }

            /* lcm_val = lcm(lcm_val, idx_seq) */
            lcm_val = lcm_seq(lcm_val, idx_seq);
        }
    }

    /* remaining prime factor > sqrt(n) (or n itself if prime) */
    if (temp > 1) {
        idx_t p = temp;
        idx_t idx = prime_index_of(p);
        seq_t idx_seq = (seq_t)idx;

        product *= idx_seq;
        lcm_val = lcm_seq(lcm_val, idx_seq);
    }

    /* The result is guaranteed to be integral */
    return product / lcm_val;
}
