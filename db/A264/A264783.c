/*
 * A264783  Primes that do not divide 2^m - 2293 for any positive integer m.
 * https://oeis.org/A264783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 2770, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A264783: primes p such that ∀m≥1, 2^m ≠ 2293 (mod p) */

#include <stdint.h>
#include <stdlib.h>

static const uint64_t LIMIT = 300000;          /* sieve bound */
static uint64_t *primes = NULL;                /* prime list */
static size_t prime_count = 0;
static int initialized = 0;

/* modular exponentiation (uint64_t) */
static uint64_t powmod_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    a %= mod;
    while (e)
    {
        if (e & 1)
            r = (r * a) % mod;
        a = (a * a) % mod;
        e >>= 1;
    }
    return r;
}

/* simple sieve to fill primes[] */
static void sieve(uint64_t limit, uint64_t **out, size_t *cnt)
{
    uint8_t *is_prime = (uint8_t*)malloc((limit + 1));
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (uint64_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    size_t sz = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++sz;

    *out = (uint64_t*)malloc(sz * sizeof(uint64_t));
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) (*out)[idx++] = i;

    *cnt = sz;
    free(is_prime);
}

/* Test whether a prime p satisfies the condition */
static int prime_ok(uint64_t p)
{
    if (p == 2) return 1;           /* 2 always included */

    uint64_t r = 2293 % p;
    uint64_t d = p - 1;             /* start with order candidate */

    uint64_t tmp = d;
    for (size_t i = 0; i < prime_count; ++i)
    {
        uint64_t q = primes[i];
        if (q * q > tmp) break;
        if (tmp % q == 0)
        {
            while (tmp % q == 0) tmp /= q;
            while (d % q == 0 && powmod_uint64(2, d / q, p) == 1)
                d /= q;
        }
    }
    if (tmp > 1)  /* remaining prime factor */
    {
        while (d % tmp == 0 && powmod_uint64(2, d / tmp, p) == 1)
            d /= tmp;
    }

    /* d is the order of 2 modulo p. r in <2> iff r^d ≡ 1. */
    return (powmod_uint64(r, d, p) != 1);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!initialized)
    {
        sieve(LIMIT, &primes, &prime_count);
        initialized = 1;
    }

    size_t found = 0;

    /* search within the precomputed primes */
    for (size_t i = 0; i < prime_count; ++i)
    {
        uint64_t p = primes[i];
        if (prime_ok(p))
        {
            ++found;
            if (found == n) return (seq_t)p;
        }
    }

    /* if not enough, continue searching beyond LIMIT */
    uint64_t candidate = LIMIT + 1;
    while (1)
    {
        /* primality test by trial division */
        int is_prime = 1;
        if ((candidate & 1) == 0 && candidate != 2) is_prime = 0;
        else
        {
            for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= candidate; ++i)
            {
                if (candidate % primes[i] == 0) { is_prime = 0; break; }
            }
        }

        if (is_prime && prime_ok(candidate))
        {
            ++found;
            if (found == n) return (seq_t)candidate;
        }
        ++candidate;
    }
    /* unreachable */
    return 0;
}
