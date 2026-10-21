/*
 * A243408  Primes p such that 10*p-1, 10*p-3, 10*p-7 and 10*p-9 are all
 *          prime.
 * https://oeis.org/A243408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _modmul, _modpow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243408 : primes p such that 10p-1, 10p-3, 10p-7 and 10p-9
   are all prime.  a(1) = 2. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/_modmul.h"
#include "../utils/_modpow.h"

/* Small prime list for quick divisibility tests */
static const uint32_t _small_primes[] = {
    2,3,5,7,11,13,17,19,23,29,31,37
};
static const size_t _small_primes_cnt = sizeof(_small_primes)/sizeof(_small_primes[0]);

/* Deterministic Miller–Rabin primality test for n < 2^32
   witnesses: 2, 7, 61 */
static bool _is_prime32(uint32_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < _small_primes_cnt; ++i) {
        uint32_t p = _small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    uint32_t d = n - 1;
    size_t s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    const uint32_t witnesses[3] = {2, 7, 61};
    for (size_t i = 0; i < 3; ++i) {
        uint32_t a = witnesses[i];
        if (a % n == 0) continue;
        uint32_t x = _modpow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (size_t r = 1; r < s; ++r) {
            x = _modmul(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Global state for sieve, prime list, and the sought sequence         */
/* ------------------------------------------------------------------ */
static uint32_t * _prime_list = NULL;
static size_t     _prime_cnt  = 0;
static seq_t * _good_list = NULL;
static size_t   _good_cnt  = 0;

/* Forward declaration */
static void _init_sequence(void);

/* Return the n-th element of the sequence (1‑based) */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    _init_sequence();
    if ((size_t)(n - 1) >= _good_cnt) return (seq_t)0;   /* beyond known values */
    return _good_list[n - 1];
}

/* ------------------------------------------------------------------ */
/* Helper: create prime list up to LIMIT (inclusive) using a sieve     */
/* ------------------------------------------------------------------ */
static void _build_prime_sieve(void)
{
    const size_t LIMIT = 50000000u;          /* 50 million: covers >10 000 terms */
    unsigned char *sieve = (unsigned char *)malloc(LIMIT + 1);
    memset(sieve, 1, LIMIT + 1);
    sieve[0] = sieve[1] = 0;

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = 0;

    /* Estimate upper bound for number of primes: roughly limit / log(limit) */
    size_t estimate = (size_t)(LIMIT / 5);   /* generous */
    _prime_list = (uint32_t *)malloc(estimate * sizeof(uint32_t));
    _prime_cnt  = 0;

    for (size_t i = 2; i <= LIMIT; ++i)
        if (sieve[i]) {
            if (_prime_cnt == estimate) {
                estimate <<= 1;
                _prime_list = (uint32_t *)realloc(_prime_list,
                                                  estimate * sizeof(uint32_t));
            }
            _prime_list[_prime_cnt++] = (uint32_t)i;
        }

    free(sieve);
}

/* ------------------------------------------------------------------ */
/* Build the good list: primes p where 10p-1,3,7,9 are all prime       */
/* ------------------------------------------------------------------ */
static void _build_good_list(void)
{
    _good_cnt = 0;
    _good_list = NULL;

    for (size_t i = 0; i < _prime_cnt; ++i) {
        uint32_t p = _prime_list[i];
        uint32_t a1 = 10u * p - 1;
        uint32_t a3 = 10u * p - 3;
        uint32_t a7 = 10u * p - 7;
        uint32_t a9 = 10u * p - 9;

        if (_is_prime32(a1) && _is_prime32(a3) &&
            _is_prime32(a7) && _is_prime32(a9)) {
            if (_good_cnt == 0) {
                /* initial capacity: a few thousand is enough */
                size_t cap = 20000;
                _good_list = (seq_t *)malloc(cap * sizeof(seq_t));
                /* unlikely to exceed; will resize if necessary */
            } else if (_good_cnt == 20000) {
                /* In the extremely rare event we need more than 20k entries,
                   double the capacity. */
                size_t cap = _good_cnt + (_good_cnt >> 1) + 1;
                _good_list = (seq_t *)realloc(_good_list, cap * sizeof(seq_t));
            }
            _good_list[_good_cnt++] = (seq_t)p;
        }
    }
}

/* ------------------------------------------------------------------ */
/* One‑time initialization of the sequence                             */
/* ------------------------------------------------------------------ */
static void _init_sequence(void)
{
    if (_prime_list != NULL) return;  /* already initialized */
    _build_prime_sieve();
    _build_good_list();
}
