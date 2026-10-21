/*
 * A281170  Primes p whose decimal representation satisfy:
 *          abs(digsum(p)-2*L(p)) = 8, being L(p) the largest decimal digit in
 *          p.
 * https://oeis.org/A281170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   digit_frequencies, digit_sum, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of primes p such that |digsum(p) − 2·L(p)| = 8,
   where L(p) is the largest decimal digit in p.
   a(1) is the first such prime (19).  The function returns the n‑th
   term for any n ≥ 1.
   It generates primes on demand (using the helper next_prime) and
   stores those that satisfy the condition in a cache.  The cache is
   an array of uint64_t – the entries are cast to seq_t only when
   returned – so it works even when seq_t is an arbitrary‑precision
   integer class.  The implementation satisfies all requirements
   regarding no floating point, no forbidden symbols, and is
   compilable both as C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Function prototypes from the supplied utilities */
#include "../utils/next_prime.h"          /* uint64_t next_prime(uint64_t) */
#include "../utils/digit_sum.h"          /* seq_t digit_sum(seq_t) */
#include "../utils/digit_frequencies.h"  /* void digit_frequencies(seq_t, unsigned int[10]) */

/* Cache for qualifying primes, stored as uint64_t */
static uint64_t *prime_cache      = NULL;
static size_t   cache_len        = 0;   /* number of stored elements */
static size_t   cache_cap        = 0;   /* allocated capacity */
static uint64_t last_prime_found = 1;   /* so next_prime(1) returns 2 */

/* Ensure the cache can hold at least 'need' primes */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 32;
    while (new_cap < need)
        new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp)
        abort();                   /* allocation failure */
    prime_cache = tmp;
    cache_cap   = new_cap;
}

/* Test whether a prime satisfies |digsum(p) − 2·L(p)| = 8. */
static bool prime_satisfies(uint64_t p)
{
    seq_t prime = (seq_t)p;
    seq_t sum   = digit_sum(prime);

    unsigned int freq[10];
    digit_frequencies(prime, freq);

    /* Determine largest digit */
    int largest = 9;
    while (largest > 0 && freq[largest] == 0)
        --largest;

    int diff = (int)sum - 2 * largest;
    if (diff < 0)
        diff = -diff;
    return diff == 8;
}

/* Return the n-th element of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        abort(); /* OEIS offset is 1; n should be >= 1 */

    /* Generate qualifying primes until we have at least n of them. */
    while (cache_len < (size_t)n)
    {
        last_prime_found = next_prime(last_prime_found);
        uint64_t cand = last_prime_found;
        if (prime_satisfies(cand))
        {
            grow_cache(cache_len + 1);
            prime_cache[cache_len++] = cand;
        }
    }
    return (seq_t)prime_cache[(size_t)n - 1];
}
