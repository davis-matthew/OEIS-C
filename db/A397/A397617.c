/*
 * A397617  a(n) = prime(floor(n*e)).
 * https://oeis.org/A397617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(floor(n*e))
 * Range:     u8 n <= 20, u16 n <= 2407, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_array = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Integer upper bound for the n-th prime.
   For n >= 6, p_n < n (log n + log log n) < n*20 for all practical n. */
static idx_t prime_bound(idx_t n) {
    if (n < 6) return 15;
    /* n*20 may overflow idx_t; use 128-bit intermediate. */
    __uint128_t prod = (__uint128_t)n * 20;
    if (prod > (idx_t)-1) return (idx_t)-1;
    return (idx_t)prod;
}

static void ensure_primes(idx_t required) {
    if (required <= prime_count) return;
    idx_t new_limit = prime_bound(required);
    if (new_limit == prime_limit) {
        /* increase limit until we have enough primes. */
        while (prime_count < required) new_limit *= 2;
    }
    if (prime_array) free(prime_array);
    build_primes_upto(new_limit, &prime_array, &prime_count);
    prime_limit = new_limit;
}

static const unsigned long long E_NUM = 2718281828459045235ULL; /* e * 1e18  */
static const unsigned long long E_DEN = 1000000000000000000ULL; /* 1e18 */

seq_t a(idx_t n) {
    /* compute floor(n*e) using rational approximation e ≈ E_NUM/E_DEN */
    __uint128_t prod = (__uint128_t)n * E_NUM;
    idx_t idx = (idx_t)(prod / E_DEN); /* idx is >= 1 for n>=1 */

    /* retrieve idx-th prime (1‑based). */
    ensure_primes(idx);
    return (seq_t)prime_array[idx - 1];
}
