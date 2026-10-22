/*
 * A374919  Primes p such that -(p - 1)^p == p (mod 2*p - 1).
 * https://oeis.org/A374919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p such that -(p‑1)^p ≡ p (mod (2p‑1))
 * Range:     u8 n <= 5, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Small helper – binary modular exponentiation using 128‑bit
 * arithmetic when available to avoid overflow. */
static uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1)
        return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Growing static cache of found primes. */
    static seq_t *list = NULL;
    static size_t size = 0;
    static size_t cap  = 0;
    static uint64_t last_checked = 1;          /* last candidate examined */

    /* Ensure we have at least n terms. */
    while (size < (size_t)n)
    {
        ++last_checked;

        /* Skip even numbers except 2. */
        if (last_checked > 2 && (last_checked & 1ULL) == 0)
            continue;

        /* Check primality using the provided helper. */
        if (!is_prime(last_checked))
            continue;

        /* Evaluate the modular condition. */
        uint64_t p   = last_checked;
        uint64_t mod = 2 * p - 1;          /* modulus */
        uint64_t pow_res = modpow_uint64(p - 1, p, mod);
        if ((pow_res + p) % mod == 0)
        {
            /* Grow the cache if necessary. */
            if (size + 1 > cap)
            {
                cap = cap ? 2 * cap : 8;
                list = (seq_t *)realloc(list, cap * sizeof(seq_t));
                if (!list)
                    exit(EXIT_FAILURE);
            }
            /* Store the prime. */
            list[size++] = (seq_t)p;
        }
    }

    return list[n - 1];
}
