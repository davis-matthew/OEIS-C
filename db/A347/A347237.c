/*
 * A347237  Möbius transform of A347236.
 * https://oeis.org/A347237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 9235, u64 n <= 9235,
 *            u128 n <= 9235, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the Dirichlet convolution of A003972 (prime‑shifted φ) with A061019,
 *            equivalently a(multiplicative) with
 *            a(p^e) = (-p)^e + (next_prime(p)-1)*(next_prime(p)^e-(-p)^e)/(next_prime(p)+p)
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* include utilities for exact integer operations */
#include "../utils/big_pow.h"

/* ---------- prime sieve ---------- */
#define PRIME_SIEVE_LIMIT 200000        /* sufficient for all n up to 10 000 */
#define MAX_PRIME_COUNT 20000

static bool _prime_sieve_initialized = false;
static uint32_t _prime_list[MAX_PRIME_COUNT];
static size_t _prime_count = 0;

/* initialise the simple sieve */
static void _init_prime_sieve(void)
{
    if (_prime_sieve_initialized) return;
    bool is_prime[PRIME_SIEVE_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_prime[j] = false;
        }
    }
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i]) _prime_list[_prime_count++] = (uint32_t)i;
    _prime_sieve_initialized = true;
}

/* next_prime(p): the least prime > p */
static uint32_t _next_prime(uint32_t p)
{
    _init_prime_sieve();
    for (size_t i = 0; i < _prime_count; ++i) {
        if (_prime_list[i] > p) return _prime_list[i];
    }
    /* fallback: trial division beyond sieve range */
    uint32_t candidate = _prime_list[_prime_count - 1] + 1;
    if (candidate % 2 == 0) ++candidate;                  /* make it odd */
    for (;; candidate += 2) {
        bool prime = true;
        for (size_t i = 0; i < _prime_count && _prime_list[i] * _prime_list[i] <= candidate; ++i)
            if (candidate % _prime_list[i] == 0) { prime = false; break; }
        if (prime) return candidate;
    }
}

/* factorise n into primes and exponents: primes[], exps[], sz */
static void _factorise(uint64_t n, uint32_t *primes, unsigned *exps, size_t *sz)
{
    _init_prime_sieve();
    *sz = 0;
    for (size_t i = 0; i < _prime_count && (uint64_t)_prime_list[i] * _prime_list[i] <= n; ++i) {
        uint32_t p = _prime_list[i];
        if (n % p == 0) {
            unsigned e = 0;
            while (n % p == 0) { n /= p; ++e; }
            primes[*sz] = p;
            exps[*sz] = e;
            ++(*sz);
        }
    }
    if (n > 1) { /* remaining prime factor */
        primes[*sz] = (uint32_t)n;
        exps[*sz]   = 1;
        ++(*sz);
    }
}

/* compute a(p^e) using the closed formula */
static seq_t _a_prime_power(uint32_t p, unsigned e)
{
    /* T1 = (-p)^e  (signed) */
    unsigned __int128 p_pow = 1;
    for (unsigned i = 0; i < e; ++i) p_pow *= p;
    __int128 term1 = (e & 1) ? -(__int128)p_pow : (__int128)p_pow;

    /* T2 = (mp-1)*(mp^e - term1)/(mp+p)  (mp = next_prime(p)) */
    uint32_t mp = _next_prime(p);
    seq_t mp_pow = big_pow((seq_t)mp, (seq_t)e);
    __int128 num = (__int128)(mp - 1) * ( ( __int128)mp_pow - term1 );
    __int128 denom = (__int128)(mp + p);
    __int128 term2 = num / denom;   /* division is exact */

    __int128 result = term1 + term2;
    return (seq_t)result;   /* final result is guaranteed non‑negative */
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                 /* out of defined range */
    if (n == 1) return (seq_t)1;                /* a(1) = 1 */

    uint64_t nn = (uint64_t)n;
    uint32_t primes[16];
    unsigned exps[16];
    size_t sz = 0;

    _factorise(nn, primes, exps, &sz);

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < sz; ++i) {
        prod *= _a_prime_power(primes[i], exps[i]);
    }
    return prod;
}
