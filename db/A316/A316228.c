/*
 * A316228  Numbers whose Fermi-Dirac prime factorization sums to a
 *          Fermi-Dirac prime.
 * https://oeis.org/A316228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 4147, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_prime_big, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find numbers whose distinct Fermi‑Dirac prime factors sum to a Fermi‑Dirac prime. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/seq_mul.h"
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_big.h"

/* exponentiation for seq_t */
static inline seq_t pow_uint(seq_t base, unsigned exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1U)
            result = seq_mul(result, base);
        base = seq_mul(base, base);
        exp >>= 1U;
    }
    return result;
}

/* sum of the distinct FD primes in the factorization of n (n ≥ 2) */
static seq_t sum_fd_prime_factors(uint64_t n)
{
    seq_t sum = (seq_t)0;
    uint64_t temp = n;

    for (uint64_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            unsigned e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            for (unsigned bit = 0; (1ULL << bit) <= e; ++bit)
                if (e & (1U << bit))
                    sum += pow_uint((seq_t)p, 1U << bit);
        }
    }
    if (temp > 1)   /* remaining prime factor */
        sum += (seq_t)temp;   /* exponent 1 ⇒ 2^0 = 1 */

    return sum;
}

/* true if x is a Fermi‑Dirac prime: p^(2^k) with p prime, k >= 0 */
static bool is_fd_prime(seq_t x)
{
    if (x <= (seq_t)1)
        return false;
    if (is_prime_big(x))
        return true;

    seq_t t = x;
    while (true)
    {
        seq_t m = floor_sqrt(t);
        if (seq_mul(m, m) != t)
            break;
        t = m;
    }
    return is_prime_big(t);
}

/* sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t candidate = 2;   /* next integer to test */

    if (n == 0)
        return (seq_t)0;   /* safety for offset handling */

    while (cache_len < n)
    {
        uint64_t num = candidate;
        seq_t sum = sum_fd_prime_factors(num);

        if (is_fd_prime(sum))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_arr)
                {
                    /* allocation failure: fallback to abort */
                    abort();
                }
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)num;
        }
        ++candidate;
    }
    return cache[n - 1];
}
