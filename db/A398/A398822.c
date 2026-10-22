/*
 * A398822  Numbers whose powerful part (A057521) is a cube that is larger
 *          than 1.
 * https://oeis.org/A398822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 5555, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Enumerate integers, keep those whose prime exponents are
   either 1 or a positive multiple of 3, and contain at least one exponent
   that is a multiple of 3.  Terms are memoised for efficient repeated
   or out‑of‑order queries.  a(1)=8, a(2)=24, a(3)=27, ... */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define PRIMES_MAX 200000

static bool primes_initialized = false;
static size_t primes_cnt = 0;
static uint64_t primes[20000];          /* π(200000) ≈ 17984 */

static void build_primes(void)
{
    if (primes_initialized)
        return;
    static bool is_prime[PRIMES_MAX + 1];
    for (size_t i = 0; i <= PRIMES_MAX; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint64_t i = 2; i * i <= PRIMES_MAX; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= PRIMES_MAX; j += i)
                is_prime[j] = false;
    for (uint64_t i = 2; i <= PRIMES_MAX; ++i)
        if (is_prime[i])
            primes[primes_cnt++] = i;
    primes_initialized = true;
}

/* Test whether x satisfies the exponent condition. */
static bool is_valid_candidate(uint64_t x)
{
    uint64_t t = x;
    bool cube_present = false;
    for (size_t idx = 0; idx < primes_cnt && primes[idx] * primes[idx] <= t; ++idx) {
        uint64_t p = primes[idx];
        int exp = 0;
        while (t % p == 0) {
            t /= p;
            ++exp;
        }
        if (exp) {
            if (exp == 1)
                continue;
            else if (exp % 3 == 0)
                cube_present = true;
            else
                return false;
        }
    }
    /* Remaining t>1 is a prime with exponent 1. */
    return cube_present;
}

/* Memoised terms. */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static uint64_t next_candidate = 2;

/* Ensure capacity for at least 'needed' elements. */
static void ensure_terms_capacity(size_t needed)
{
    if (needed <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    if (new_cap < needed)
        new_cap = needed;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                    /* undefined for 0, returns 0 */

    build_primes();

    while (terms_len < (size_t)n) {         /* generate until we have n terms */
        if (is_valid_candidate(next_candidate)) {
            ensure_terms_capacity(terms_len + 1);
            terms[terms_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return terms[n - 1];
}
