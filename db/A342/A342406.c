/*
 * A342406  Primes p such that the number of ways to write p = 2*q + r with q
 *          and r prime is a prime.
 * https://oeis.org/A342406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that the number of ways to write p
 *            = 2*q + r with q and r prime is a prime number
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t *prime_list = NULL;   /* list of primes discovered so far    */
static size_t prime_cap  = 0;
static size_t prime_cnt  = 0;

static uint64_t *cache_vals = NULL;   /* cache of sequence terms (uint64)    */
static size_t cache_cap  = 0;
static size_t cache_cnt  = 0;

static uint64_t next_candidate = 2;   /* next integer to test for primality */

/* Ensure that an array has room for at least 'need' elements.            */
static void ensure_cap(size_t *cap, void **arr, size_t elem_size, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap) ? (*cap << 1) : 16;
        while (new_cap < need) new_cap <<= 1;
        *arr = realloc(*arr, new_cap * elem_size);
        *cap = new_cap;
    }
}

/* Append a new prime to prime_list                                 */
static void add_prime(uint64_t p)
{
    ensure_cap(&prime_cap, (void **)&prime_list, sizeof(uint64_t), prime_cnt + 1);
    prime_list[prime_cnt++] = p;
}

/* Append a new term to the cache                                    */
static void add_cache(uint64_t p)
{
    ensure_cap(&cache_cap, (void **)&cache_vals, sizeof(uint64_t), cache_cnt + 1);
    cache_vals[cache_cnt++] = p;
}

/* Return the n-th term (1‑based).  The function is memoised so any
   sequence of calls works correctly.                                 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* guard for zero index */

    while (cache_cnt < (size_t)n) {
        /* ---- Find the next prime p ------------------------------------ */
        uint64_t p = 0;
        for (;;) {
            if (is_prime(next_candidate)) {
                p = next_candidate;
                add_prime(p);
                next_candidate++;
                break;
            }
            next_candidate++;
        }

        /* ---- Count representations p = 2*q + r ------------------------ */
        uint64_t count = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t q = prime_list[i];
            if (2 * q > p - 2) break;          /* q > (p-2)/2   */
            uint64_t r = p - 2 * q;
            if (is_prime(r)) count++;
        }

        /* ---- If the count is prime, store this prime in the cache ---- */
        if (is_prime(count)) {
            add_cache(p);
        }
    }

    return (seq_t)cache_vals[n - 1];
}
