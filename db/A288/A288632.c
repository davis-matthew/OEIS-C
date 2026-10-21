/*
 * A288632  Primes p such that (p+q)/6 is prime, q is the next prime after p.
 * https://oeis.org/A288632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 372, u32 n <= 82, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A288632 – primes p such that (p+q)/6 is prime, q is the next prime after p. */
/*  This implementation generates successive primes, tests the condition,
    and memoises the sequence so that each call to a(idx_t n) retrieves the
    n-th term (1‑based).  It works for any bit‑width seq_t and idx_t.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t n);      */
#include "../utils/next_prime.h"       /* uint64_t next_prime(uint64_t);  */

/* ---------- static cache for the sequence ---------- */
static seq_t  *seq_cache   = NULL;   /* array of cached terms   */
static size_t seq_cap     = 0;        /* current capacity        */
static size_t seq_len     = 0;        /* number of terms cached */

/* Current prime being examined; initialise to the first prime. */
static seq_t current_prime = 2;       /* will become the first prime after the loop starts */

/* Ensure the cache has room for at least 'new_cap' terms. */
static void ensure_seq_capacity(size_t new_cap)
{
    if (new_cap <= seq_cap)
        return;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* Return the n-th element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined below offset */

    /* Initialise cache on first call. */
    if (!seq_cache) {
        seq_cache = (seq_t *)malloc(32 * sizeof(seq_t));
        seq_cap   = 32;
    }

    /* If requested term already cached, return it. */
    if ((size_t)n <= seq_len)
        return seq_cache[n - 1];

    /* Generate primes until the desired index is reached. */
    while (seq_len < (size_t)n) {
        /* Find next prime q after current_prime.  The helper next_prime
           returns the smallest prime > start, so we can use it directly. */
        uint64_t cp_val = (uint64_t)current_prime;
        uint64_t q64 = next_prime(cp_val);
        seq_t q = (seq_t)q64;

        /* Compute (p+q)/6 and test primality of both the ratio and the
           prime status of p itself (by construction p is prime). */
        seq_t sum = current_prime + q;
        if ((sum % (seq_t)6) == (seq_t)0) {
            seq_t ratio = sum / (seq_t)6;
            if (is_prime_big(ratio)) {
                /* Cache the term p. */
                if (seq_len == seq_cap)
                    ensure_seq_capacity(seq_cap * 2 + 16);
                seq_cache[seq_len++] = current_prime;
            }
        }

        /* Advance to next prime. */
        current_prime = q;
    }

    return seq_cache[n - 1];
}
