/*
 * A318660  Remainder when A064988(n) is divided by n.
 * https://oeis.org/A318660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∏_{p^e‖n} prime(p)^e) mod n where prime(p) denotes the
 *            p‑th prime number (1‑indexed)
 * Range:     u8 n <= 284, u16 n <= 9596, u32 n <= 9596, u64 n <= 9596,
 *            u128 n <= 9596, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Edge case: a(1) = 0.  Computation uses only integral operations
    and the helper `build_primes_upto` for generating primes up to a
    fixed limit. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static const idx_t PRIMES_LIMIT = 200000;   /* enough primes for p ≤ 10000 */

static void init_primes(void)
{
    if (prime_count == 0)
    {
        build_primes_upto(PRIMES_LIMIT, &prime_list, &prime_count);
        /* guaranteed that prime_count ≥ 10000 ≥ max prime index needed */
    }
}

static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp > 0)
    {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                /* a(1) = 0 by definition */

    init_primes();

    uint64_t temp   = (uint64_t)n;
    uint64_t result = 1 % n;

    /* factorize n and accumulate the product of prime(p)^e modulo n */
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= temp; ++i)
    {
        uint64_t p = prime_list[i];
        if (temp % p == 0)
        {
            uint64_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            if (p == 0) continue;          /* safety check, never happens */

            /* p is itself a prime; obtain the p‑th prime */
            uint64_t base  = prime_list[p - 1];          /* 1‑indexed */
            uint64_t powmod = mod_pow_uint64(base, e, n);
            result = (result * powmod) % n;
        }
    }

    /* remaining prime factor greater than sqrt(original n) */
    if (temp > 1)
    {
        uint64_t p = temp;            /* leftover prime factor */
        uint64_t base = prime_list[p - 1];   /* base = prime(p) */
        uint64_t powmod = base % n;
        result = (result * powmod) % n;
    }

    return (seq_t)result;
}
