/*
 * A339905  Fully multiplicative with a(prime(k)) = prime(k+1) - 1.
 * https://oeis.org/A339905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (nextprime(p)-1)^e , a(1)=1 Fully
 *            multiplicative with a(prime_k) = prime_{k+1}-1
 * Range:     u8 n <= 80, u16 n <= 6560, u32 n <= 9815, u64 n <= 9815,
 *            u128 n <= 9815, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define PRIME_LIMIT 20000          /* >= next prime after any prime <= 10000 */
#define MAX_PRIMES  3000           /* safe upper bound for number of primes */

static uint8_t sieve[PRIME_LIMIT + 1];
static uint32_t primes[MAX_PRIMES];
static size_t   prime_count = 0;
static int      primes_built = 0;

/* Build a list of primes up to PRIME_LIMIT using the sieve of Eratosthenes. */
static void build_primes(void)
{
    if (primes_built)
        return;

    sieve[0] = sieve[1] = 1;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 1;
        }
    }
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i])
            primes[prime_count++] = (uint32_t)i;

    primes_built = 1;
}

/* Return the prime immediately following p.  It is assumed that p is prime. */
static uint32_t next_prime_of(uint32_t p)
{
    for (size_t i = 0; i < prime_count; ++i) {
        if (primes[i] == p) {
            /* The table contains plenty of primes so next should exist */
            return primes[i + 1];
        }
    }
    /* Fallback: find next prime by trial division (should not be reached). */
    uint32_t candidate = p + 1;
    while (1) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= candidate; ++i) {
            if (candidate % primes[i] == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime)
            return candidate;
        ++candidate;
    }
}

/* Factor n into primes; compute the multiplicative value as defined. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)1;

    build_primes();

    seq_t result = (seq_t)1;
    uint64_t tmp = (uint64_t)n;      /* work on 64-bit temp */

    for (size_t pi = 0; pi < prime_count; ++pi) {
        uint32_t p = primes[pi];
        if ((uint64_t)p * p > tmp)
            break;
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            uint32_t next = next_prime_of(p);
            seq_t base  = (seq_t)(next - 1);
            for (int e = 0; e < exp; ++e)
                result *= base;
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1) {
        uint32_t p = (uint32_t)tmp;
        uint32_t next = next_prime_of(p);
        seq_t base  = (seq_t)(next - 1);
        result *= base;
    }

    return result;
}
