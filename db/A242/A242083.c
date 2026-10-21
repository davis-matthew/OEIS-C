/*
 * A242083  a(n) = 3^p - 2^p - 1, where p is prime(n).
 * https://oeis.org/A242083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^prime(n) - 2^prime(n) - 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers for memory allocation and basic types */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Big integer helper */
#include "../utils/big_pow.h"

/* ------------------------------------------------------------------ */
/* Prime sieve infrastructure: generate primes on demand (1‑based). */
/* ------------------------------------------------------------------ */

static uint64_t *prime_list    = NULL;   /* array of discovered primes   */
static size_t    prime_count   = 0;      /* number of primes found       */
static size_t    prime_capacity = 0;    /* current allocated capacity   */
static uint64_t  last_trial     = 1;     /* largest tested candidate     */

/* Ensure prime_list has at least 'min_cap' elements of space. */
static void prime_grow_capacity(size_t min_cap) {
    if (min_cap <= prime_capacity)
        return;
    size_t new_cap = prime_capacity ? prime_capacity * 2 : 16;
    if (new_cap < min_cap)
        new_cap = min_cap;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    /* In practice realloc will never fail in the test harness. */
    prime_list    = tmp;
    prime_capacity = new_cap;
}

/* Return the nth prime (1‑based).  The prime is always <= uint64_t. */
static uint64_t get_nth_prime(idx_t n) {
    if (n == 0)                     /* undefined; avoid UB – safe fallback   */
        return 0;
    if (prime_count >= (size_t)n)   /* already computed                         */
        return prime_list[n-1];

    /* Need to find more primes.  Expand storage a bit. */
    prime_grow_capacity(prime_count + 128);

    uint64_t candidate = last_trial + 1;
    while (prime_count < (size_t)n) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p > candidate / p)      /* p^2 > candidate  */
                break;
            if (candidate % p == 0) {   /* composite */
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_count == prime_capacity)
                prime_grow_capacity(prime_count + 16);
            prime_list[prime_count++] = candidate;
        }
        ++candidate;
    }
    last_trial = candidate - 1;
    return prime_list[n-1];
}

/* ------------------------------------------------------------------ */
/* OEIS sequence function a(n)  (n >= 1)                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0)               /* behavior below the offset is irrelevant */
        return 0;
    uint64_t p = get_nth_prime(n);    /* prime(n) as a uint64_t */

    seq_t pow3 = big_pow((seq_t)3, (seq_t)p);   /* 3^p  */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)p);   /* 2^p  */
    seq_t result = pow3 - pow2 - 1;             /* 3^p - 2^p - 1 */

    return result;
}
