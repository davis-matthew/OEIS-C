/*
 * A241809  Semiprimes sp such that sp+2 is a prime.
 * https://oeis.org/A241809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1370, u32 n <= 1370, u64 n <= 1370,
 *            u128 n <= 1370, big any n
 * Helpers:   _smallest_prime_factor, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Semiprimes sp such that sp+2 is a prime (OEIS A241809).
 * This implementation generates the terms by searching successive
 * integers and testing for semiprimality and primality of sp+2.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"  /* uint64_t _smallest_prime_factor(uint64_t n); */
#include "../utils/is_prime.h"                /* bool is_prime(idx_t x); */

/* static cache of found terms (a(n) for n>=1) */
static seq_t *cache = NULL;
static size_t cache_len   = 0;   /* number of terms stored */
static size_t cache_cap   = 0;   /* current capacity of the array */
static idx_t  last_checked = 0;  /* last integer examined */

/* enlarge cache when needed */
static void ensure_cache_capacity(void)
{
    if (cache_len >= cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof *tmp);
        if (!tmp) {
            /* fatal error – abort */
            abort();
        }
        cache = tmp;
    }
}

/* generate semiprimes until we have at least target terms */
static void generate_up_to(size_t target)
{
    idx_t x = last_checked;
    while (cache_len < target) {
        ++x;
        last_checked = x;

        /* reject primes */
        if (_smallest_prime_factor(x) == x)
            continue;

        idx_t p = _smallest_prime_factor(x);
        if (p == 0)                /* should not happen, but guard */
            continue;
        idx_t q = x / p;            /* cofactor */

        if (!is_prime(q))          /* not exactly two prime factors */
            continue;

        /* check that x+2 is prime */
        if (!is_prime(x + 2))
            continue;

        ensure_cache_capacity();
        /* cast to seq_t – assume value fits in the type */
        cache[cache_len++] = (seq_t)x;
    }
}

/* external entry point required by the build system */
seq_t a(idx_t n)
{
    if (n < 1)                /* indices below offset are undefined */
        return (seq_t)0;

    /* ensure we have computed enough terms */
    generate_up_to((size_t)n);

    return cache[n - 1];
}
