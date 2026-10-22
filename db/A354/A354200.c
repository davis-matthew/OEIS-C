/*
 * A354200  Prime map that sends 2 to 5, and each 4k+1 and 4k+3 prime to the
 *          next larger prime of the same type.
 * https://oeis.org/A354200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: generate primes up to a suitable limit and map the n‑th prime
   to the next larger prime of the same residue modulo 4 (with 2↦5).  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static uint64_t *prime_list = NULL;        /* list of primes */
static size_t prime_count  = 0;           /* number of primes stored */
static size_t prime_limit  = 0;           /* upper bound of current sieve */

/* Append a prime to the dynamic array. */
static void append_prime(uint64_t p)
{
    if (prime_count == 0 || prime_count % 64 == 0) {
        size_t new_cap = prime_count == 0 ? 64 : prime_count * 2;
        uint64_t *new_arr = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!new_arr) abort();
        prime_list = new_arr;
    }
    prime_list[prime_count++] = p;
}

/* Sieve of Eratosthenes up to 'limit', rebuild the prime list. */
static void build_primes_up_to(size_t limit)
{
    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime) abort();
    for (size_t i = 2; i <= limit; ++i) is_prime[i] = true;

    for (size_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (size_t q = p * p; q <= limit; q += p) is_prime[q] = false;
        }
    }

    /* free previous list */
    free(prime_list);
    prime_list = NULL;
    prime_count = 0;

    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) append_prime(i);

    free(is_prime);
    prime_limit = limit;
}

/* Estimate an upper bound for the n‑th prime (valid for n>=6). */
static size_t estimate_limit_for_n(idx_t n)
{
    if (n < 6) return 15;  /* covers primes up to 13 */
    double nn   = (double)n;
    double est  = nn * (log(nn) + log(log(nn)));
    return (size_t)(est + 3 * nn);  /* small safety margin */
}

/* Ensure that we have at least 'n' primes in the list. */
static void ensure_primes_for_n(idx_t n)
{
    if (prime_count >= (size_t)n) return;
    size_t lim = estimate_limit_for_n(n);
    if (lim <= prime_limit) lim = prime_limit * 2;
    build_primes_up_to(lim);
    /* keep widening until we have enough primes */
    while (prime_count < (size_t)n) {
        lim *= 2;
        build_primes_up_to(lim);
    }
}

/* Binary search for the index of a prime in the prime list. */
static size_t find_prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_count - 1;
    while (lo <= hi) {
        size_t mid = (lo + hi) / 2;
        if (prime_list[mid] == p) return mid;
        if (prime_list[mid] < p) lo = mid + 1;
        else if (mid == 0) break;   /* avoid underflow */
        else hi = mid - 1;
    }
    return prime_count; /* not found (should not happen) */
}

/* Map a prime as described in the OEIS entry. */
static seq_t map_prime(seq_t p)
{
    if (p == (seq_t)2) return (seq_t)5;
    int mod = (int)(p & 3);          /* 1 or 3 */
    size_t idx = find_prime_index((uint64_t)p);
    for (size_t i = idx + 1; i < prime_count; ++i) {
        if ((prime_list[i] & 3) == mod) return (seq_t)prime_list[i];
    }
    /* If we run out of primes (rare for n<10000), extend the sieve. */
    size_t ext_limit = prime_limit * 2;
    build_primes_up_to(ext_limit);
    return map_prime(p);
}

/* Required OEIS function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but keep sane */
    ensure_primes_for_n(n);
    seq_t p = (seq_t)prime_list[n - 1];  /* A000040(n): the n-th prime */
    return map_prime(p);
}
