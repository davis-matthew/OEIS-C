/*
 * A340053  First of three consecutive primes p,q,r such that p*q*r mod (p+q),
 *          p*q*r mod (p+r) and p*q*r mod (q+r) are all primes.
 * https://oeis.org/A340053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 320, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A340053: first of every three consecutive primes p,q,r such that
   p*q*r mod (p+q), p*q*r mod (p+r) and p*q*r mod (q+r) are all primes. */
/* The function builds primes as needed, tests each consecutive triple,
   caches qualifying p values, and returns the n-th term for any n ≥ 1. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

/* Cached sequence values */
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static seq_t *seq_cache = NULL;

/* Cached primes */
static uint64_t *prime_arr   = NULL;
static size_t   prime_cnt    = 0;
static idx_t    prime_limit  = 0;   /* current max prime limit */
static bool     primes_built = false;
static size_t   prime_next   = 0;   /* next triple start index */

/* Ensure a sufficient prime table is available. */
static void ensure_primes(void)
{
    if (!primes_built) {
        prime_limit = (idx_t)20000000;               /* reasonable start */
        build_primes_upto(prime_limit, &prime_arr, &prime_cnt);
        primes_built = true;
    }
}

/* Resize cache if necessary and append a new element. */
static void cache_append(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap == 0 ? 1024 : seq_cap * 2;
        seq_t *new_ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = new_ptr;
        seq_cap = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* Test if the triple (p,q,r) satisfies the definition. */
static bool triple_qualifies(uint64_t p, uint64_t q, uint64_t r)
{
    /* compute product modulo each sum using 128‑bit arithmetic */
    const uint64_t m1 = p + q;
    const uint64_t m2 = p + r;
    const uint64_t m3 = q + r;

    __int128 prod = (__int128)p * q % m1;
    prod = prod * r % m1;
    if (!is_prime((idx_t)(uint64_t)prod)) return false;

    prod = (__int128)p * q % m2;
    prod = prod * r % m2;
    if (!is_prime((idx_t)(uint64_t)prod)) return false;

    prod = (__int128)p * q % m3;
    prod = prod * r % m3;
    if (!is_prime((idx_t)(uint64_t)prod)) return false;

    return true;
}

/* Generate terms until we have at least n cached. */
static void generate_up_to(idx_t n)
{
    ensure_primes();

    while (seq_len < n) {
        /* If we are near the end of the current prime table, enlarge it. */
        if (prime_next + 2 >= prime_cnt) {
            /* free old table */
            free(prime_arr);
            /* increase limit and rebuild */
            prime_limit = prime_limit * 2 + 1000;
            build_primes_upto(prime_limit, &prime_arr, &prime_cnt);
        }

        uint64_t p = prime_arr[prime_next];
        uint64_t q = prime_arr[prime_next + 1];
        uint64_t r = prime_arr[prime_next + 2];
        if (triple_qualifies(p, q, r))
            cache_append((seq_t)p);
        prime_next++;
    }
}

/* Main accessor for the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n=0, but harmless */
    generate_up_to(n);
    return seq_cache[n - 1];
}
