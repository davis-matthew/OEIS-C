/*
 * A345382  Numbers with exactly 3 semiprime divisors.
 * https://oeis.org/A345382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This sequence consists of numbers that are either the product of
 *            three distinct primes, or the product of two primes each raised
 *            to at least the second power
 * Range:     u8 n <= 34, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   ull_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/ull_sqrt.h"

/* ---------- prime table utilities ---------- */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0, prime_cap = 0;
static uint64_t prime_limit = 1;

/* ensure that 'primes' contains all primes ≤ limit */
static void ensure_prime_limit(uint64_t limit)
{
    if (limit <= prime_limit)
        return;
    uint64_t new_limit = limit;

    bool *is_comp = (bool *)malloc((new_limit + 1) * sizeof(bool));
    if (!is_comp)
        return;                    /* allocation failure */

    for (uint64_t i = 0; i <= new_limit; ++i)
        is_comp[i] = false;
    is_comp[0] = is_comp[1] = true;

    uint64_t sqrt_lim = ull_sqrt(new_limit);
    for (uint64_t i = 2; i <= sqrt_lim; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = i * i; j <= new_limit; j += i)
                is_comp[j] = true;
        }
    }

    size_t new_cnt = prime_cnt;
    for (uint64_t i = 2; i <= new_limit; ++i) {
        if (!is_comp[i]) {
            if (new_cnt >= prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
                if (!tmp)
                    return;
                primes = tmp;
            }
            primes[new_cnt++] = i;
        }
    }
    prime_cnt = new_cnt;
    prime_limit = new_limit;

    free(is_comp);
}

/* factorisation: returns number of distinct primes (r) and number of exponents ≥2 (b) */
static void factor_properties(uint64_t m, size_t *r, size_t *b)
{
    *r = 0;
    *b = 0;
    uint64_t temp = m;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > temp / p)
            break;
        if (temp % p == 0) {
            ++(*r);
            size_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (exp >= 2)
                ++(*b);
        }
    }
    if (temp > 1)
        ++(*r);                     /* remaining prime factor with exponent 1 */
}

/* ---------- cache for sequence values ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static uint64_t next_candidate = 2; /* next integer to test */

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined for n = 0 */

    /* ensure at least a trivial prime for factorisation */
    ensure_prime_limit(2);

    /* grow cache until it contains at least n entries */
    while (cache_len < (size_t)n) {
        uint64_t m = next_candidate++;

        uint64_t lim = ull_sqrt(m);
        ensure_prime_limit(lim);

        size_t r = 0, b = 0;
        factor_properties(m, &r, &b);

        if ((r == 3 && b == 0) || (r == 2 && b == 2)) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)          /* allocation failure – return 0 as sentinel */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)m;
        }
    }
    return cache[n - 1];
}
