/*
 * A319525  Completely multiplicative with a(prime(k)) = prime(2*k - 1) (where
 *          prime(k) denotes the k-th prime).
 * https://oeis.org/A319525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a is a completely multiplicative function with a(prime(k)) =
 *            prime(2*k-1) (prime(k) = k-th prime). For n=1 it returns 1. The
 *            implementation is efficient for n up to about 10 000 and can be
 *            called in any order
 * Range:     u8 n <= 44, u16 n <= 1874, u32 n <= 9220, u64 n <= 9220,
 *            u128 n <= 9220, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- local prime generation (once) ---------- */
#define SIEVE_LIMIT 40000
static uint32_t primes[SIEVE_LIMIT];
static size_t prime_count;
static uint16_t prime_index_of[SIEVE_LIMIT + 1];

static void init_primes(void)
{
    bool is_prime[SIEVE_LIMIT + 1];
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = false;
        }
    }
    prime_count = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            primes[prime_count++] = (uint32_t)i;
            prime_index_of[i] = (uint16_t)(prime_count);   /* 1‑based index */
        }
    }
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    /* lazy initialisation of prime tables */
    static bool initialized = false;
    if (!initialized) {
        init_primes();
        initialized = true;
    }

    if (n == 0)          /* not defined for 0, but return 0 */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    idx_t tmp = n;       /* copy because we will change it in factorisation */
    seq_t result = (seq_t)1;

    /* factorise tmp using the pre‑computed primes */
    for (size_t pi = 0; pi < prime_count &&
                      (uint64_t)primes[pi] * primes[pi] <= tmp; ++pi) {
        uint32_t p = primes[pi];
        if (tmp % p == 0) {
            size_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            /* a(p) = prime(2*index(p) - 1) */
            uint16_t k = prime_index_of[p];          /* 1‑based index of p */
            seq_t base = (seq_t)primes[2 * k - 2];   /* prime(2*k-1) */
            for (size_t i = 0; i < exp; ++i)
                result *= base;
        }
    }
    if (tmp > 1) {   /* remaining prime factor */
        uint32_t p = (uint32_t)tmp;
        uint16_t k = prime_index_of[p];
        seq_t base = (seq_t)primes[2 * k - 2];
        result *= base;   /* exponent is 1 */
    }

    return result;
}
