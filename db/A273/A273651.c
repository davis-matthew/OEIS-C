/*
 * A273651  a(n) = A000594(p) mod p, where p = prime(n).
 * https://oeis.org/A273651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000594(prime(n)) mod prime(n) Recurrence: a(1) = 0 For n
 *            > 1: a(n) = -1680 * Σ_{i=1..(p-1)/2} i⁴ * σ(i) * σ(p-i) (mod p),
 *            where p = prime(n) and σ(k) = sum of divisors of k
 * Range:     u8 n <= 63, u16 n <= 6632, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global tables for primes and sigma values up to a fixed limit. */
static uint64_t *primes      = NULL;
static size_t     primes_cnt = 0;

static uint64_t *sigma_arr  = NULL;
static size_t     sigma_cap = 0;

/* Simple sieve that fills primes[] with all primes up to 'limit'. */
static void init_primes(size_t limit)
{
    bool *comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (!comp) exit(EXIT_FAILURE);

    for (size_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            /* Grow primes array if needed. */
            if (primes_cnt == 0 || primes_cnt % 256 == 0) {
                size_t new_cap = (primes_cnt == 0) ? 256 : primes_cnt * 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                if (!tmp) { free(comp); exit(EXIT_FAILURE); }
                primes = tmp;
            }
            primes[primes_cnt++] = i;

            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    comp[j] = true;
            }
        }
    }
    free(comp);
}

/* Divisor‑sieve to compute σ(k) for 1 ≤ k ≤ limit. */
static void init_sigma(size_t limit)
{
    sigma_arr = (uint64_t *)calloc(limit + 1, sizeof(uint64_t));
    if (!sigma_arr) exit(EXIT_FAILURE);

    for (size_t i = 1; i <= limit; ++i) {
        for (size_t j = i; j <= limit; j += i)
            sigma_arr[j] += i;
    }
    sigma_cap = limit;
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        /* Enough to cover prime(10000) ≈ 104729. */
        size_t limit = 200000;
        init_primes(limit);
        init_sigma(limit);
        initialized = true;
    }

    if (n <= 0) return 0;          /* Guard for non‑positive indices. */
    uint64_t p = primes[n - 1];     /* primes[] is 0‑indexed. */
    uint64_t mod = p;
    uint64_t sum = 0;

    /* Summation over i = 1 .. (p‑1)/2. */
    for (idx_t i = 1; i <= (p - 1) / 2; ++i) {
        uint64_t ii = (uint64_t)i;
        uint64_t i2 = (ii * ii) % mod;
        uint64_t i4 = (i2 * i2) % mod;
        uint64_t si = sigma_arr[i] % mod;
        uint64_t sj = sigma_arr[p - i] % mod;

        __uint128_t term = (__uint128_t)i4 * si;
        term %= mod;
        term = (term * sj) % mod;

        sum += (uint64_t)term;
        if (sum >= mod) sum -= mod;
    }

    /* factor = (-1680) mod p */
    uint64_t factor = (mod - (1680 % mod)) % mod;
    uint64_t result = (uint64_t)(((__uint128_t)factor * sum) % mod);

    return (seq_t)result;
}
