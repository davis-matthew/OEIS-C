/*
 * A296926  Rational primes that decompose in the field Q(sqrt(-13)).
 * https://oeis.org/A296926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 3254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth rational prime that splits in the
   quadratic field Q(√‑13).  The splitting primes are exactly the
   primes p≠13 with p ≡ 1,7,9,11,15,17,19,25,29,31,47,49 (mod 52). */
/* The implementation uses the supplied `next_prime` routine and
   builds a small, cached table of the required primes.  The table
   grows only when a larger index is requested.  Any `seq_t` type
   that can be constructed from an uint64_t (including the arbitrary‑
   precision big integer supplied by the build) will accept the
   returned value. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"          /* provides uint64_t next_prime(uint64_t) */

/* residue classes modulo 52 that correspond to splitting primes */
static const uint8_t residue_classes[12] = {
    1, 7, 9, 11, 15, 17, 19, 25, 29, 31, 47, 49
};

/* cached data */
static uint64_t *splitting_primes = NULL;  /* table of primes (uint64_t) */
static idx_t      count_cached   = 0;      /* number of primes stored */
static idx_t      capacity_cached= 0;      /* current allocated capacity */
static uint64_t   last_prime     = 2;      /* largest prime examined so far */

/* check whether a prime p satisfies the residue condition */
static bool
is_splitting_prime(uint64_t p)
{
    uint8_t r = (uint8_t)(p % 52);
    for (size_t i = 0; i < 12; ++i)
        if (r == residue_classes[i])
            return true;
    return false;
}

/* grow the cached array if necessary */
static void
ensure_capacity(idx_t needed)
{
    if (needed <= capacity_cached)
        return;
    idx_t new_cap = (capacity_cached == 0) ? 64 : capacity_cached * 2;
    while (new_cap < needed)
        new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(splitting_primes,
                                        new_cap * sizeof(uint64_t));
    if (!tmp) {
        /* fatal, but unlikely for the required limits */
        exit(EXIT_FAILURE);
    }
    splitting_primes = tmp;
    capacity_cached  = new_cap;
}

/* fill the cache until it contains at least n primes */
static void
make_cache_at_least(idx_t n)
{
    if (count_cached >= n)
        return;
    ensure_capacity(n);
    while (count_cached < n) {
        last_prime = next_prime(last_prime + 1);
        if (is_splitting_prime(last_prime)) {
            splitting_primes[count_cached++] = last_prime;
        }
    }
}

/* ------------------------------------------------------------ */
/* public entry point required by the build system */

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 1, safeguard */
    make_cache_at_least(n);
    return (seq_t)splitting_primes[n - 1];
}
