/*
 * A273960  a(n) = (-1)^n*prime(n).
 * https://oeis.org/A273960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * prime(n)
 * Range:     i8 n <= 31, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th signed prime.  Uses a simple incremental prime
   generator stored in a static cache.  Works for any build of seq_t,
   idx_t = uint64_t.  No external helper functions are required. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Static cache of primes */
static uint64_t *prime_cache   = NULL;   /* array of primes */
static size_t   prime_cnt      = 0;      /* number of primes stored */
static size_t   prime_cap      = 0;      /* current allocated capacity */
static idx_t    last_checked   = 1;      /* last integer tested for primality */

/* Ensure that prime_cache contains at least n primes (n ≥ 1). */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= n)
        return;

    /* Initialise cache if needed. */
    if (prime_cache == NULL) {
        prime_cap = 128;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    /* Incrementally generate primes until we have at least n. */
    while (prime_cnt < n) {
        /* Find next candidate integer to test. */
        if (last_checked < 2)
            last_checked = 2;
        else if (last_checked == 2)
            last_checked = 3;
        else
            last_checked += 2;          /* skip even numbers */

        uint64_t cand = last_checked;
        bool is_prime = true;

        /* Test divisibility by already known primes up to sqrt(cand). */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_cache[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store the new prime. */
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                prime_cache = (uint64_t *)realloc(prime_cache, prime_cap * sizeof(uint64_t));
            }
            prime_cache[prime_cnt++] = cand;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for offset 1, return 0 just for safety */
        return (seq_t)0;

    ensure_primes(n);

    seq_t p   = (seq_t)prime_cache[n - 1];
    seq_t sign = (n & 1ULL) ? (seq_t)(-1) : (seq_t)1;

    return p * sign;
}
