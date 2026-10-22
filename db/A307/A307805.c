/*
 * A307805  a(n) = first position of prime(n) in A023503.
 * https://oeis.org/A307805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1874, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes OEIS sequence A307805:
 * a(n) = smallest i such that the largest prime factor of (prime(i) - 1)
 *        equals prime(n).
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"  /* uint32_t get_prime(idx_t k) */
#include "../utils/gpf.h"        /* uint64_t gpf(uint64_t x) */

/* static cache: first_i_cache[p] = first index i with gpf(prime(i)-1)=p.
 * 0 indicates “not yet seen” */
static seq_t *first_i_cache = NULL;   /* dynamic array of indices */
static size_t  cache_size    = 0;      /* allocated array length */
static idx_t   last_checked_i = 0;     /* maximal i processed so far */

/* Ensure the cache can hold entries up to 'need' (inclusive). */
static void
ensure_cache_size(size_t need)
{
    if (need <= cache_size)
        return;
    size_t new_size = need;
    if (new_size < 64)
        new_size = 64;
    while (new_size < need)
        new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(first_i_cache, new_size * sizeof(seq_t));
    if (!tmp)
        abort();  /* out of memory – terminate */
    for (size_t i = cache_size; i < new_size; ++i)
        tmp[i] = 0;  /* sentinel “not seen” */
    first_i_cache = tmp;
    cache_size    = new_size;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1 */

    /* Prime corresponding to index n */
    uint32_t prime_n = get_prime(n);

    /* The cache index is the prime value, so enlarge if necessary. */
    ensure_cache_size((size_t)prime_n + 1);

    if (first_i_cache[prime_n] != 0)
        return first_i_cache[prime_n];

    /* Scan primes beyond what we have processed so far until we find a match */
    for (idx_t i = last_checked_i + 1; ; ++i)
    {
        uint32_t pi = get_prime(i);                       /* prime(i) */
        uint64_t g  = gpf((uint64_t)pi - 1ULL);           /* gpf of prime(i)-1 */
        size_t gi   = (size_t)g;

        /* Expand cache to hold this prime value if needed */
        if (gi >= cache_size)
            ensure_cache_size(gi + 1);

        /* Record first occurrence of g if not already stored */
        if (first_i_cache[gi] == 0)
            first_i_cache[gi] = (seq_t)i;

        /* Did we find the requested prime n? */
        if (gi == prime_n)
        {
            first_i_cache[prime_n] = (seq_t)i;
            if (i > last_checked_i)
                last_checked_i = i;
            break;
        }

        if (i > last_checked_i)
            last_checked_i = i;
    }

    return first_i_cache[prime_n];
}
