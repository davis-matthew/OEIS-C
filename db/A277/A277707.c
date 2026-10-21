/*
 * A277707  a(n) = index of the least prime divisor of n which has an odd
 *          exponent, or 0 if n is a perfect square.
 * https://oeis.org/A277707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the least prime divisor of n with odd exponent,
 *            * or 0 if n is a perfect square. * The index uses OEIS prime
 *            indexing where prime(1)=2
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Static state for primes up to 10000 */
static bool primes_initialized = false;
static size_t prime_count = 0;
static uint64_t primes[2000];   /* enough for primes <= 10000 */
static idx_t *prime_index = NULL;  /* prime_index[p] = OEIS index of prime p, 0 if not prime */
static size_t prime_index_size = 0;

static void init_primes(void)
{
    if (primes_initialized) return;

    const idx_t LIMIT = 10000;

    /* Simple sieve */
    bool *is_prime = (bool *)malloc((size_t)(LIMIT + 1) * sizeof(bool));
    if (!is_prime) return; /* should never happen */
    for (size_t i = 0; i <= (size_t)LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i) is_prime[j] = false;
        }
    }

    prime_index_size = (size_t)LIMIT + 1;
    prime_index = (idx_t *)calloc(prime_index_size, sizeof(idx_t));
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            primes[prime_count] = (uint64_t)i;
            prime_index[i] = (idx_t)(prime_count + 1); /* OEIS index starts at 1 */
            ++prime_count;
        }
    }

    free(is_prime);
    primes_initialized = true;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;  /* a(1) = 0, and any n < 2 is 1 or 0 */

    init_primes();

    idx_t temp = n;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;

        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (exp & 1) {  /* odd exponent found */
                return (seq_t)prime_index[p];
            }
            /* If even exponent, continue searching */
        }
    }

    if (temp > 1) {
        /* Remaining factor is prime with exponent 1 (odd) */
        return (seq_t)prime_index[temp];
    }

    /* All exponents were even: perfect square */
    return (seq_t)0;
}
