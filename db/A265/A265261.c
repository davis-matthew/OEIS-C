/*
 * A265261  Smallest n-Knödel number, i.e., smallest composite c > n such that
 *          each b < c coprime to c satisfies b^(c-n) == 1 (mod c).
 * https://oeis.org/A265261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest composite c > n such that λ(c) | (c-n) where
 *            λ(c) is the Carmichael function (exponent of (ℤ/cℤ)×)
 * Range:     u8 none, u16 n <= 4000, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   build_primes_upto, gcd, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b) */
#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t num) */
#include "../utils/build_primes_upto.h"/* void build_primes_upto(idx_t limit,
                                        uint64_t **primes_out, size_t *cnt_out) */

static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

/* Initialize the global list of primes once */
static void init_primes(void)
{
    if (prime_list != NULL)
        return;               /* already initialized */
    /* A generous upper bound keeps the search fast for n ≤ 10000.
       1 000 000 primes up to 1 000 000 are enough for the intended range. */
    build_primes_upto(1000000, &prime_list, &prime_cnt);
    prime_limit = 1000000;
}

/* Least common multiple for two 64‑bit integers using the gcd helper */
static uint64_t lcm_uint64(uint64_t a, uint64_t b)
{
    if (a == 0 || b == 0) return 0;
    return a / (uint64_t)gcd((seq_t)a, (seq_t)b) * b;
}

/* Compute Carmichael λ(n) for a composite n (64‑bit).  */
static uint64_t lambda_of(uint64_t n)
{
    uint64_t result = 1;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i)
    {
        uint64_t p = prime_list[i];
        if (temp % p == 0)
        {
            uint64_t pow = p;
            unsigned exp = 1;
            temp /= p;
            while (temp % p == 0)
            {
                ++exp;
                temp /= p;
                pow *= p;
            }
            uint64_t part;
            if (p == 2)
            {
                if (exp == 1)      part = 1;          /* λ(2) = 1   */
                else if (exp == 2) part = 2;          /* λ(4) = 2   */
                else               part = pow >> 2;   /* λ(2ᵏ) = 2ᵏ⁻², k≥3 */
            }
            else
            {
                part = pow - pow / p;                /* λ(pᵏ) = φ(pᵏ) */
            }
            result = lcm_uint64(result, part);
        }
    }
    if (temp > 1)         /* remaining prime factor */
    {
        uint64_t part = (temp == 2) ? 1U : temp - 1U;
        result = lcm_uint64(result, part);
    }
    return result;
}

/* Compute the A265261 term for a given index n ≥ 1. */
seq_t a(idx_t n)
{
    init_primes();
    uint64_t nn = (uint64_t)n;
    uint64_t c = nn + 1;        /* start checking from n+1 */
    for (;; ++c)
    {
        /* Skip primes, we need a composite number. */
        if (c > 1 && !is_prime_small((seq_t)c))
        {
            uint64_t lambda = lambda_of(c);
            if ((c - nn) % lambda == 0)
                return (seq_t)c;     /* found the smallest such c */
        }
    }
}
