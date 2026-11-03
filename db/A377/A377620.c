/*
 * A377620  a(n) is the number of iterations of x -> 5*x + 4 until (#
 *          composites reached) = (# primes reached), starting with prime(n).
 * https://oeis.org/A377620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of x -> 5*x + 4 until the counts of
 *            primes and composites encountered in the chain starting from
 *            prime(n) are equal (OEIS A377620)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 55, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 85 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* static state: primes table and a(n) cache */
static uint64_t *prime_cache = NULL;   /* prime_cache[i] = (i+1)-th prime */
static size_t prime_cnt    = 0;        /* number of primes stored so far */

/* Builds a sieve up to at least LIMIT, storing all primes <= LIMIT.
   If a larger limit than the previous one is requested, the sieve is rebuilt. */
static void build_primes(size_t limit) {
    if (prime_cache != NULL && prime_cnt >= limit) return;

    /* free old cache */
    free(prime_cache);
    prime_cache = NULL;
    prime_cnt = 0;

    /* simple bit sieve */
    size_t sieve_size = limit + 1;
    uint8_t *sieve = (uint8_t *)calloc(sieve_size, sizeof(uint8_t));
    if (!sieve) return; /* allocation failure, leave cache empty */

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i) sieve[j] = 1;
        }
    }

    /* count primes */
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i) if (!sieve[i]) ++count;
    prime_cache = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!prime_cache) { free(sieve); sieve = NULL; return; }

    /* store primes */
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) prime_cache[idx++] = (uint64_t)i;
    }
    prime_cnt = count;
    free(sieve);
}

/* Ensure that prime_cache contains at least the first n primes.
   For n <= 10000 the bound n*20 is safely above the n-th prime. */
static void ensure_primes(idx_t n) {
    if (prime_cnt >= (size_t)n) return;
    size_t new_limit = (n < 6) ? 15 : (size_t)(n * 20);
    build_primes(new_limit);
}

/* a(n) cache */
static seq_t *a_cache     = NULL;
static size_t a_cache_sz = 0;

static void ensure_a_cache(idx_t n) {
    if (a_cache_sz >= (size_t)n) return;
    size_t new_sz = (size_t)n;
    seq_t *new_arr = mem_resize(seq_t, a_cache, new_sz);
    if (!new_arr) return; /* allocation failure */
    for (size_t i = a_cache_sz; i < new_sz; ++i) new_arr[i] = 0; /* 0 means not yet computed */
    a_cache     = new_arr;
    a_cache_sz = new_sz;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;               /* undefined for non‑positive indices */

    ensure_primes(n);
    ensure_a_cache(n);
    if (a_cache[n - 1] != 0) return a_cache[n - 1];

    seq_t x   = (seq_t)prime_cache[n - 1];
    seq_t pc  = 1;      /* primes seen */
    seq_t cc  = 0;      /* composites seen */
    seq_t it  = 0;      /* iterations performed */

    while (pc != cc) {
        x = 5 * x + 4;
        ++it;
        if (isPrimeGeneric(x))
            ++pc;
        else
            ++cc;
    }

    a_cache[n - 1] = it;
    return it;
}
